// *****************************************************************************
//
// $Created: NLD 2003-11-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Config.cpp $
// $Author: Nld $
// $Modtime: 15/04/05 17:05 $
// $Revision: 6 $
// $Workfile: MIL_Config.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Config.h"
#include "MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;



// -----------------------------------------------------------------------------
// Name: MIL_Config constructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::MIL_Config()
    : bCheckPointOrbat_         ( false )
    , bUseCheckPointCRC_        ( true )
    , bUseOnlyDIAArchive_       ( false )
    , bCheckAutomateComposition_( false )
    , bUseDecDebug_             ( false )
    , bUsePathDebug_            ( false )
    , bUseDiaDebugger_          ( false )
    , diaDebuggerPort_          ( 0 )
    , bUseNetworkLogger_        ( false )
    , networkLoggerPort_        ( 0 )
    , bProfilingEnabled_        ( false )
    , bDataTestMode_            ( false )
    , bEmbeddedDispatcher_      ( false )
{
    po::options_description desc( "Simulation options" );
    desc.add_options()
        ( "checkpointorbat",                                                  "use backup orbat with checkpoint"         )
        ( "test"           ,                                                  "test mode: loading + first tick"          )
        ( "testdata"       ,                                                  "test mode: load models only (no terrain)" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config destructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::~MIL_Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::Parse( int argc, char** argv )
{
    tools::SessionConfig::Parse( argc, argv );
    bDataTestMode_    = IsSet( "testdata" );
    bTestMode_        = bDataTestMode_ || IsSet( "test" );
    bCheckPointOrbat_ = IsSet( "checkpointorbat" );
    ReadSessionFile( GetSessionFile() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadSessionFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadSessionFile( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "session" )
        >> xml::start( "config" )
            >> xml::start( "simulation" )
                >> xml::start( "decisional" )
                    >> xml::attribute( "useonlybinaries", bUseOnlyDIAArchive_ )
                >> xml::end()
                >> xml::start( "orbat" )
                    >> xml::attribute( "checkcomposition", bCheckAutomateComposition_ )
                >> xml::end()
                >> xml::start( "profiling" )
                    >> xml::attribute( "enabled", bProfilingEnabled_ )
                >> xml::end()
                >> xml::start( "dispatcher" )
                    >> xml::attribute( "embedded", bEmbeddedDispatcher_ )
                >> xml::end()
                >> xml::start( "network" )
                    >> xml::attribute( "port", networkPort_ )
                >> xml::end()
                >> xml::start( "time" )
                    >> xml::attribute( "step", timeStep_ )
                    >> xml::attribute( "factor", timeFactor_ )
                >> xml::end()
                >> xml::start( "pathfinder" )
                    >> xml::attribute( "threads", pathFinderThreads_ )
                >> xml::end()
                >> xml::start( "hla" )
                    >> xml::attribute( "enabled"   , bHLAEnabled_ )
                    >> xml::attribute( "federation", hlaFederation_ )
                    >> xml::attribute( "federate"  , hlaFederate_ )
                >> xml::end();

    ReadCheckPointConfiguration( xis );
    ReadDebugConfiguration     ( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadCheckPointConfiguration
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadCheckPointConfiguration( xml::xistream& xis )
{
    std::string frequency;
    xis >> xml::start( "checkpoint" )
            >> xml::attribute( "frequency", frequency )
            >> xml::attribute( "keep", checkPointsKept_ )
            >> xml::attribute( "usecrc", bUseCheckPointCRC_ )
        >> xml::end();
    if( !tools::DecodeTime( frequency, checkPointsFrequency_ ) )
        xis.error( "Invalid time specified for checkpoint frequency" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadDebugConfiguration
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadDebugConfiguration( xml::xistream& xis )
{
    xis >> xml::start( "debug" )
            >> xml::attribute( "decisional", bUseDecDebug_ )
            >> xml::attribute( "pathfind", bUsePathDebug_ )
            >> xml::attribute( "diadebugger", bUseDiaDebugger_ )
            >> xml::optional() >> xml::attribute( "diadebuggerport", diaDebuggerPort_ )
            >> xml::attribute( "networklogger", bUseNetworkLogger_ )
            >> xml::optional() >> xml::attribute( "networkloggerport", networkLoggerPort_ )
        >> xml::end();
    if( bUseDiaDebugger_ && !diaDebuggerPort_ )
        throw std::exception( "DIA debug server activated but no debugger port specified!" );
    if( bUseNetworkLogger_ && !networkLoggerPort_ )
        throw std::exception( "Network logger activated but no port specified!" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::AddFileToCRC
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
void MIL_Config::AddFileToCRC( const std::string& fileName )
{
    if( CRCMap_.find( fileName ) == CRCMap_.end() )
        CRCMap_[ fileName ] = MIL_Tools::ComputeCRC( fileName );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::serialize
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
boost::crc_32_type::value_type MIL_Config::serialize( const std::string& strFileName ) const
{
    try
    {
        xml::xofstream xos( strFileName );

        xos << xml::start( "files" );
        for( CIT_CRCMap it = CRCMap_.begin(); it != CRCMap_.end(); ++it )
            xos << xml::start( "file" )
                    << xml::attribute( "name", it->first )
                    << xml::attribute( "crc", it->second )
                << xml::end();
        xos << xml::end();
    }
    catch( xml::exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot create file '%s'", strFileName.c_str() ), e.what() );
    }
    return MIL_Tools::ComputeCRC( strFileName );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetOrbatFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::GetOrbatFile() const
{
    if( bCheckPointOrbat_ )
        return BuildCheckpointChildFile( "orbat.xml" );
    return tools::SessionConfig::GetOrbatFile();
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::BuildCheckpointChildFile
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string MIL_Config::BuildCheckpointChildFile( const std::string& file, std::string name /*= ""*/ ) const
{
    if( name.empty() )
        return BuildDirectoryFile( GetCheckpointDirectory(), file );
    return BuildDirectoryFile( tools::SessionConfig::GetCheckpointDirectory( name ), file );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsPopulation
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
bool MIL_Config::IsPopulationEnabled() const
{
    return ! GetPopulationFile().empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPopulationDir
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
std::string MIL_Config::GetPopulationDir() const
{
    if( IsPopulationEnabled() )
        return BuildChildPath( GetPopulationFile(), "." );
    return "";
}
