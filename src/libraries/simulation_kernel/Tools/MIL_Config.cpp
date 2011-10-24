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
#include "tools/Loader.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable: 4127 4512 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: MIL_Config constructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::MIL_Config( tools::RealFileLoaderObserver_ABC& observer )
    : tools::SessionConfig      ( observer )
    , endTick_                  ( 0 )
    , diaDebuggerPort_          ( 0 )
    , networkLoggerPort_        ( 0 )
    , networkPort_              ( 0 )
    , networkTimeOut_           ( 10000 )
    , bCheckPointOrbat_         ( false )
    , bUseCheckPointCRC_        ( true )
    , bUseOnlyDIAArchive_       ( false )
    , bCheckAutomateComposition_( false )
    , bUseDecDebug_             ( false )
    , bUsePathDebug_            ( false )
    , bUseDiaDebugger_          ( false )
    , bUseNetworkLogger_        ( false )
    , bProfilingEnabled_        ( false )
    , bDataTestMode_            ( false )
    , bSaveCheckpointTestMode_  ( false )
    , bDeleteCheckpointTestMode_( false )
    , bFrozenMode_              ( false )
    , bEmbeddedDispatcher_      ( false )
    , bPausedAtStartup_         ( false )
    , randomSeed_               ( 0 )
{
    po::options_description desc( "Simulation options" );
    desc.add_options()
        ( "checkpointorbat"                                          , "use backup orbat with checkpoint"         )
        ( "test"                                                     , "test mode: loading + first tick"          )
        ( "testdata"                                                 , "test mode: load models only (no terrain)" )
        ( "savecheckpoint" , po::value( &strCheckPointNameTestMode_ ), "specify checkpoint to save"               )
        ( "deletecheckpoint"                                         , "delete checkpoint folder"                 )
        ( "simulation-port", po::value( &networkPort_ )              , "specify the simulation server port number"      );
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
    bDataTestMode_ = IsSet( "testdata" );
    bTestMode_ = bDataTestMode_ || IsSet( "test" );
    bSaveCheckpointTestMode_ = IsSet( "savecheckpoint" );
    bCheckPointOrbat_ = IsSet( "checkpointorbat" );
    bDeleteCheckpointTestMode_ = IsSet( "deletecheckpoint" );
    ReadSessionFile( GetSessionFile() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadSessionFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadSessionFile( const std::string& file )
{
    setpause_ = 100;
    setstepmul_ = 200;
    GetLoader().LoadFile( file, boost::bind( &MIL_Config::ReadSessionXml, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadSessionXml
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void MIL_Config::ReadSessionXml( xml::xistream& xis )
{
    unsigned short port;
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "simulation" )
                    >> xml::start( "decisional" )
                        >> xml::attribute( "useonlybinaries", bUseOnlyDIAArchive_ )
                    >> xml::end
                    >> xml::optional
                    >> xml::start( "experimental" )
                        >> xml::attribute( "frozenmode", bFrozenMode_ )
                    >> xml::end
                    >> xml::start( "orbat" )
                        >> xml::attribute( "checkcomposition", bCheckAutomateComposition_ )
                    >> xml::end
                    >> xml::start( "profiling" )
                        >> xml::attribute( "enabled", bProfilingEnabled_ )
                    >> xml::end
                    >> xml::start( "dispatcher" )
                        >> xml::attribute( "embedded", bEmbeddedDispatcher_ )
                    >> xml::end
                    >> xml::start( "network" )
                        >> xml::attribute( "port", port )
                        >> xml::optional >> xml::attribute( "timeout", networkTimeOut_ )
                    >> xml::end
                    >> xml::start( "time" )
                        >> xml::attribute( "step", timeStep_ )
                        >> xml::attribute( "factor", timeFactor_ )
                        >> xml::optional >> xml::attribute( "end-tick", endTick_ )
                        >> xml::optional >> xml::attribute( "paused", bPausedAtStartup_ )
                    >> xml::end
                    >> xml::optional
                    >> xml::start( "GarbageCollector" )
                        >> xml::attribute( "setpause", setpause_ )
                        >> xml::attribute( "setstepmul", setstepmul_ )
                    >> xml::end
                    >> xml::start( "pathfinder" )
                        >> xml::attribute( "threads", pathFinderThreads_ )
                    >> xml::end
                    >> xml::optional
                    >> xml::start( "random" )
                        >> xml::attribute( "seed", randomSeed_ )
                    >> xml::end;
    if( ! networkPort_ )
        networkPort_ = port;
    ConfigureRandom( xis );
    ReadCheckPointConfiguration( xis );
    ReadDebugConfiguration     ( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ConfigureRandom
// Created: SLI 2010-07-15
// -----------------------------------------------------------------------------
void MIL_Config::ConfigureRandom( xml::xistream& xis )
{
    for( int i = 0; i < MIL_Random::eContextsNbr; ++i )
    {
        randomGaussian_[ i ] = false;
        randomDeviation_[ i ] = 0.;
        randomMean_[ i ] = 0.;
        xis >> xml::optional
            >> xml::start( "random" + boost::lexical_cast< std::string >( i ) )
                >> xml::attribute( "distribution", randomGaussian_[ i ] )
                >> xml::attribute( "deviation", randomDeviation_[ i ] )
                >> xml::attribute( "mean", randomMean_[ i ] )
            >> xml::end;
    }
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
        >> xml::end;
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
            >> xml::optional >> xml::attribute( "diadebuggerport", diaDebuggerPort_ )
            >> xml::attribute( "networklogger", bUseNetworkLogger_ )
            >> xml::optional >> xml::attribute( "networkloggerport", networkLoggerPort_ )
        >> xml::end;
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
                << xml::end;
        xos << xml::end;
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
// Name: MIL_Config::GetUrbanFile
// Created: SLG 2009-10-10
// -----------------------------------------------------------------------------
std::string MIL_Config::GetUrbanFile() const
{
    return tools::SessionConfig::GetUrbanFile();
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
