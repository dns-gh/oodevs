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
#include "xeumeuleu/xml.h"

#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_Config constructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::MIL_Config()
    : tools::GeneralConfig      ()
    , strCheckPointName_        ()
    , bCheckPointOrbat_         ( false )
    , bCheckPoint_              ( false )
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
    , CRCMap_                   ()
{
    po::options_description desc( "Simulation options" );
    desc.add_options()
        ( "checkpoint"     , po::value< std::string >( &strCheckPointName_ ), "specify checkpoint to load"               )
        ( "checkpointorbat", po::value< std::string >( &strCheckPointName_ ), "use backup orbat with checkpoint"         )
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
// Name: MIL_Config::ReadGameFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadGameFile( const std::string& file )
{
    xifstream xis( file );
    xis >> start( "config" )
            >> start( "simulation" )
                >> start( "decisional" )
                    >> attribute( "useonlybinaries", bUseOnlyDIAArchive_ )
                >> end()
                >> start( "orbat" )
                    >> attribute( "checkcomposition", bCheckAutomateComposition_ )
                >> end()
                >> start( "profiling" )
                    >> attribute( "enabled", bProfilingEnabled_ )
                >> end()
                >> start( "dispatcher" )
                    >> attribute( "embedded", bEmbeddedDispatcher_ )
                >> end()
                >> start( "network" )
                    >> attribute( "port", networkPort_ )
                    >> attribute( "threaded", bNetworkThreaded_ )
                >> end()
                >> start( "time" )
                    >> attribute( "step", timeStep_ )
                    >> attribute( "factor", timeFactor_ )
                >> end()
                >> start( "pathfinder" )
                    >> attribute( "threads", pathFinderThreads_ )
                >> end()
                >> start( "hla" )
                    >> attribute( "enabled"   , bHLAEnabled_ )
                    >> attribute( "federation", hlaFederation_ )
                    >> attribute( "federate"  , hlaFederate_ )
                >> end();

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
    xis >> start( "checkpoint" )
            >> attribute( "frequency", frequency )
            >> attribute( "keep", checkPointsKept_ )
            >> attribute( "usecrc", bUseCheckPointCRC_ )
        >> end();
    if( !tools::DecodeTime( frequency, checkPointsFrequency_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid time specified for checkpoint frequency" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadDebugConfiguration
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadDebugConfiguration( xml::xistream& xis )
{
    xis >> start( "debug" )
            >> attribute( "decisional", bUseDecDebug_ )
            >> attribute( "pathfind", bUsePathDebug_ )
            >> attribute( "diadebugger", bUseDiaDebugger_ )
            >> optional() >> attribute( "diadebuggerport", diaDebuggerPort_ )
            >> attribute( "networklogger", bUseNetworkLogger_ )
            >> optional() >> attribute( "networkloggerport", networkLoggerPort_ )
        >> end();
    if( bUseDiaDebugger_ && !diaDebuggerPort_ )
        throw std::exception( "DIA debug server activated but no debugger port specified!" );
    if( bUseNetworkLogger_ && !networkLoggerPort_ )
        throw std::exception( "Network logger activated but no port specified!" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadExerciseFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::ReadExerciseFile()
{
    xml::xifstream xis( GetExerciseFile() );
    xis >> start( "exercise" )
            >> start( "terrain" )
                >> attribute( "name", terrain_ )
            >> end()
            >> optional() >> start( "population" )
                >> attribute( "name", population_ )
            >> end()
            >> start( "model" )
                >> attribute( "dataset", dataset_ )
                >> attribute( "physical", physical_ )
            >> end()
            >> start( "weather" )
                >> attribute( "file", weather_ )
            >> end()
            >> start( "orbat" )
                >> attribute( "file", orbat_ )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::AddFileToCRC
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
void MIL_Config::AddFileToCRC( const std::string& fileName )
{
    if ( CRCMap_.find( fileName ) == CRCMap_.end() )
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
    
        xos << start( "files" );
        for ( CIT_CRCMap it = CRCMap_.begin(); it != CRCMap_.end(); ++it )
            xos << start( "file" )
                    << attribute( "name", it->first )
                    << attribute( "crc", it->second )
                << end();
        xos << end();
    }
    catch( xml::exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Can't create file '%s'", strFileName.c_str() ), e.what() );
    }
    return MIL_Tools::ComputeCRC( strFileName );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void MIL_Config::Parse( int argc, char** argv )
{
    tools::GeneralConfig::Parse( argc, argv );
    bDataTestMode_    = IsSet( "testdata" );
    bTestMode_        = bDataTestMode_ || IsSet( "test" );
    bCheckPoint_      = IsSet( "checkpoint" );
    bCheckPointOrbat_ = IsSet( "checkpointorbat" );
    ReadGameFile( GetGameFile() );
    ReadExerciseFile();
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPhysicalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::GetPhysicalFile() const
{
    return tools::GeneralConfig::GetPhysicalFile( dataset_, physical_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::BuildPhysicalChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::BuildPhysicalChildFile( const std::string& file ) const
{
    return tools::GeneralConfig::BuildPhysicalChildFile( dataset_, physical_, file );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetDecisionalFile
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string MIL_Config::GetDecisionalFile()
{
    return tools::GeneralConfig::GetDecisionalFile( dataset_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::BuildDecisionalChildFile
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string MIL_Config::BuildDecisionalChildFile( const std::string& file ) const
{
    return tools::GeneralConfig::BuildDecisionalChildFile( dataset_, file );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetTerrainFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::GetTerrainFile() const
{
    return tools::GeneralConfig::GetTerrainFile( terrain_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::BuildTerrainChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::BuildTerrainChildFile( const std::string& file ) const
{
    return tools::GeneralConfig::BuildTerrainChildFile( terrain_, file );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetWeatherFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::GetWeatherFile() const
{
    return BuildExerciseChildFile( weather_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetOrbatFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string MIL_Config::GetOrbatFile() const
{
    if( bCheckPointOrbat_ )
        return BuildCheckpointChildFile( "orbat.xml" );
    return BuildExerciseChildFile( orbat_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::BuildCheckpointChildFile
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string MIL_Config::BuildCheckpointChildFile( const std::string& file, std::string name /*= ""*/ ) const
{
    return BuildGameChildFile( ( bfs::path( "checkpoints", bfs::native ) / bfs::path( name.empty() ?  strCheckPointName_ : name, bfs::native ) / file ).native_file_string() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsPopulation
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
bool MIL_Config::IsPopulationEnabled() const
{
    return population_.size() > 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPopualtionDir
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
std::string MIL_Config::GetPopulationDir() const
{
    return ( bfs::path( tools::GeneralConfig::GetPopualtionDir(), bfs::native ) / bfs::path( population_, bfs::native ) / bfs::path( "model", bfs::native ) ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPopualtionFile
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
std::string MIL_Config::GetPopulationFile() const
{
    if ( population_.size() > 0 )
        return "population.xml";
    return "";
}
