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
#include "tools/Codec.h"
#include "tools/Loader.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: MIL_Config constructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::MIL_Config( tools::RealFileLoaderObserver_ABC& observer )
    : tools::SessionConfig( observer )
    , checkPointsKept_( 0 )
    , checkPointsFrequency_( 0 )
    , timeStep_( 0 )
    , timeFactor_( 0 )
    , endTick_( 0 )
    , tickLatency_( 1 )
    , pathFinderThreads_( 0 )
    , networkLoggerPort_( 0 )
    , networkPort_( 0 )
    , networkTimeOut_( 10000 )
    , bCheckPointOrbat_( false )
    , bUseCheckPointCRC_( false )
    , bCheckAutomateComposition_( false )
    , bUseDecDebug_( false )
    , bUsePathDebug_( false )
    , bUseNetworkLogger_( false )
    , bDecisionalProfilingEnabled_( false )
    , bDecisionalLoggerEnabled_( false )
    , bHookProfilingEnabled_( false )
    , bCommandProfilingEnabled_( false )
    , bTestMode_( false )
    , bSaveCheckpointTestMode_( false )
    , bEmbeddedDispatcher_( false )
    , bPausedAtStartup_( false )
    , bLegacy_( false )
    , randomSeed_( 0 )
    , setpause_( 0 )
    , setstepmul_( 0 )
    , integrationDir_( "resources" )
    , createNoPartyObjects_( true )
    , subset_( false )
{
    po::options_description desc( "Simulation options" );
    desc.add_options()
        ( "checkpointorbat"                                               , "use backup orbat with checkpoint"          )
        ( "test"                                                          , "test mode: loading + first tick"           )
        ( "testdata"                                                      , "test mode: load models only (no terrain)"  )
        ( "savecheckpoint" , po::value( &strCheckPointNameTestMode_ )     , "specify checkpoint to save"                )
        ( "deletecheckpoint"                                              , "delete checkpoint folder"                  )
        ( "legacy", po::value< bool >( &bLegacy_ )->default_value( false ), "activate legacy mode"                      )
        ( "integration-dir", po::value( &integrationDir_ )                , "set integration directory"                 )
        ( "dump-pathfinds",  po::value( &pathfindDir_ )                   , "set pathfind dump directory" )
        ( "filter-pathfinds",po::value( &pathfindFilter_ )                , "set pathfind id filter, separate multiple values with commas" )
        ( "simulation-port", po::value( &networkPort_ )                   , "specify the simulation server port number" );
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
    bTestMode_ = IsSet( "test" ) || IsSet( "savecheckpoint" );
    bSaveCheckpointTestMode_ = IsSet( "savecheckpoint" );
    bCheckPointOrbat_ = IsSet( "checkpointorbat" );
    ReadSessionFile( GetSessionFile() );
    if( bTestMode_ )
    {
        bEmbeddedDispatcher_ = true;
        bPausedAtStartup_ = false;
        endTick_ = 8;
        if( IsSaveCheckpointTestMode() )
            checkPointsFrequency_ = endTick_ / 2 * timeStep_;
    }
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

namespace
{
    void ExtractParties( std::string subset, std::set< unsigned int >& parties )
    {
        std::vector< std::string > result;
        boost::split( result, subset, boost::algorithm::is_any_of( ";" ) );
        for( auto it = result.begin(); it != result.end(); ++it )
            parties.insert( boost::lexical_cast< unsigned int >( *it ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ReadSessionXml
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void MIL_Config::ReadSessionXml( xml::xistream& xis )
{
    unsigned short port;
    std::string subsetParties;
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "simulation" )
                    >> xml::start( "orbat" )
                        >> xml::attribute( "checkcomposition", bCheckAutomateComposition_ )
                        >> xml::optional
                        >> xml::start( "subset" )
                        >> xml::optional
                        >> xml::attribute( "no-party", createNoPartyObjects_ );
    subset_ = xis.has_attribute( "parties" );
    xis                 >> xml::optional
                        >> xml::attribute( "parties", subsetParties )
                        >> xml::end
                    >> xml::end
                    >> xml::start( "profiling" )
                        >> xml::optional >> xml::attribute( "enabled", bDecisionalProfilingEnabled_ )
                        >> xml::optional >> xml::attribute( "hook", bHookProfilingEnabled_ )
                        >> xml::optional >> xml::attribute( "command", bCommandProfilingEnabled_ )
                        >> xml::optional >> xml::attribute( "decisional", bDecisionalProfilingEnabled_ )
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
                        >> xml::optional >> xml::attribute( "latency", tickLatency_ )
                    >> xml::end
                    >> xml::optional
                    >> xml::start( "GarbageCollector" )
                        >> xml::attribute( "setpause", setpause_ )
                        >> xml::attribute( "setstepmul", setstepmul_ )
                    >> xml::end
                    >> xml::start( "pathfinder" )
                        >> xml::attribute( "threads", pathFinderThreads_ );
    if( xis.has_attribute( "max-calculation-time" ) )
    {
        unsigned int t;
        if( tools::ReadTimeAttribute( xis, "max-calculation-time", t ))
            pathFinderMaxComputationTime_ = t;
    }
    xis             >> xml::end
                    >> xml::optional
                    >> xml::start( "random" )
                        >> xml::attribute( "seed", randomSeed_ )
                    >> xml::end;
    if( ! networkPort_ )
        networkPort_ = port;
    if( subset_ )
        ExtractParties( subsetParties, subsetParties_ );
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
    LogSettingsData logSim;
    xis >> xml::start( "debug" )
            >> xml::attribute( "decisional", bUseDecDebug_ )
            >> xml::attribute( "pathfind", bUsePathDebug_ )
            >> xml::attribute( "networklogger", bUseNetworkLogger_ )
            >> xml::optional >> xml::attribute( "decisional-logger", bDecisionalLoggerEnabled_ )
            >> xml::optional >> xml::attribute( "networkloggerport", networkLoggerPort_ )
            >> xml::optional >> xml::attribute( "loglevel", logSim.level_ )
            >> xml::optional >> xml::attribute( "logfiles", logSim.files_ )
            >> xml::optional >> xml::attribute( "logsize", logSim.fileSize_ )
            >> xml::optional >> xml::attribute( "sizeunit", logSim.sizeUnit_ )
        >> xml::end;
    SetSimLogSettings( logSim );
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
        for( auto it = CRCMap_.begin(); it != CRCMap_.end(); ++it )
            xos << xml::start( "file" )
                    << xml::attribute( "name", it->first )
                    << xml::attribute( "crc", it->second )
                << xml::end;
        xos << xml::end;
    }
    catch( const xml::exception& e )
    {
        throw MASA_EXCEPTION( "Cannot create file '" + strFileName + "', " + tools::GetExceptionMsg( e ) );
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
// Name: MIL_Config::GetIntegrationDir
// Created: BAX 2012-11-09
// -----------------------------------------------------------------------------
const std::string& MIL_Config::GetIntegrationDir() const
{
    return integrationDir_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPathFinderMaxComputationTime
// Created: MCO 2012-12-10
// -----------------------------------------------------------------------------
boost::optional< unsigned int > MIL_Config::GetPathFinderMaxComputationTime() const
{
    return pathFinderMaxComputationTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPathfindDir
// Created: BAX 2012-12-11
// -----------------------------------------------------------------------------
const std::string& MIL_Config::GetPathfindDir() const
{
    return pathfindDir_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPathfindFilter
// Created: BAX 2012-12-11
// -----------------------------------------------------------------------------
const std::string& MIL_Config::GetPathfindFilter() const
{
    return pathfindFilter_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::CanCreateNoPartyObjects
// Created: LGY 2013-02-11
// -----------------------------------------------------------------------------
bool MIL_Config::CanCreateNoPartyObjects() const
{
    return createNoPartyObjects_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::CanCreateParty
// Created: LGY 2013-02-11
// -----------------------------------------------------------------------------
bool MIL_Config::CanCreateParty( unsigned int id ) const
{
    if( !subset_ )
        return true;
    return subsetParties_.find( id ) != subsetParties_.end();
}
