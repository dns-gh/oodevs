// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Config.h"
#include "tools/Loader.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::Config( tools::RealFileLoaderObserver_ABC& observer )
    : SessionConfig( observer )
    , networkSimulationPort_   ( 0 )
    , networkClientsParameters_( 0 )
    , networkShieldParameters_ ( 0 )
    , networkTimeout_          ( 10000 )
    , keyFramesFrequency_      ( 100 )
    , replayFragmentsFrequency_( 150 )
    , timeStep_                ( 0 )
    , reportsClearFrequency_   ( 100 )
    , useShieldUtf8Encoding_   ( true )
    , subset_                  ( false )
{
    po::options_description desc( "Dispatcher/replayer options" );
    desc.add_options()
        ( "simulation-port", po::value( &networkSimulationPort_ )   , "specify the simulation server port number" )
        ( "dispatcher-port", po::value( &networkClientsParameters_ ), "specify the dispatcher server port number" );
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

namespace
{
    void ExtractParties( const std::string& subset, std::set< unsigned int >& parties )
    {
        std::vector< std::string > result;
        boost::split( result, subset, boost::algorithm::is_any_of( ";" ) );
        for( auto it = result.begin(); it != result.end(); ++it )
            parties.insert( boost::lexical_cast< unsigned int >( *it ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Config::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void Config::Parse( int argc, char** argv )
{
    LogSettingsData logShield, logDispatcherProtobuf, logDispatcher, logLoggerPlugin;
    tools::SessionConfig::Parse( argc, argv );
    unsigned short port;
    std::string subsetParties;
    xml::xifstream xis( GetSessionFile() );
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "simulation" )
                    >> xml::start( "orbat" )
                        >> xml::optional
                        >> xml::start( "subset" );
    subset_ = xis.has_attribute( "parties" );
    xis                     >> xml::optional
                            >> xml::attribute( "parties", subsetParties )
                        >> xml::end
                    >> xml::end
                    >> xml::start( "time" )
                        >> xml::attribute( "step", timeStep_ )
                    >> xml::end
                >> xml::end
                >> xml::start( "dispatcher" )
                    >> xml::start( "network" )
                        >> xml::attribute( "client", networkSimulationParameters_ )
                        >> xml::attribute( "server", port )
                        >> xml::optional >> xml::attribute( "timeout", networkTimeout_ )
                    >> xml::end
                    >> xml::optional >> xml::start( "log" )
                        >> xml::optional >> xml::attribute( "loglevel", logDispatcher.level_ )
                        >> xml::optional >> xml::attribute( "logfiles", logDispatcher.files_ )
                        >> xml::optional >> xml::attribute( "logsize",  logDispatcher.fileSize_ )
                        >> xml::optional >> xml::attribute( "sizeunit", logDispatcher.sizeUnit_ )
                    >> xml::end
                    >> xml::optional >> xml::start( "messages" )
                        >> xml::optional >> xml::attribute( "loglevel", logLoggerPlugin.level_ )
                        >> xml::optional >> xml::attribute( "logfiles", logLoggerPlugin.files_ )
                        >> xml::optional >> xml::attribute( "logsize",  logLoggerPlugin.fileSize_ )
                        >> xml::optional >> xml::attribute( "sizeunit", logLoggerPlugin.sizeUnit_ )
                    >> xml::end
                    >> xml::optional >> xml::start( "debug" )
                        >> xml::optional >> xml::attribute( "logfiles", logDispatcherProtobuf.files_ )
                        >> xml::optional >> xml::attribute( "logsize",  logDispatcherProtobuf.fileSize_ )
                        >> xml::optional >> xml::attribute( "sizeunit", logDispatcherProtobuf.sizeUnit_ )
                    >> xml::end
                    >> xml::start( "plugins" )
                        >> xml::optional >>xml::start( "recorder" )
                            >> xml::optional >> xml::attribute( "fragmentfreq", replayFragmentsFrequency_ )
                            >> xml::optional >> xml::attribute( "keyframesfreq", keyFramesFrequency_ )
                        >> xml::end
                        >> xml::optional >>xml::start( "shield" )
                            >> xml::attribute( "server", networkShieldParameters_ )
                            >> xml::optional >> xml::attribute( "logfiles", logShield.files_ )
                            >> xml::optional >> xml::attribute( "logsize", logShield.fileSize_ )
                            >> xml::optional >> xml::attribute( "sizeunit", logShield.sizeUnit_ )
                            >> xml::optional >> xml::attribute( "use-utf8-string-encoding", useShieldUtf8Encoding_ )
                        >> xml::end
                    >> xml::end
                    >> xml::optional >> xml::start( "reports" )
                        >> xml::attribute( "frequency", reportsClearFrequency_ )
                    >> xml::end;
    if( subset_ )
        ExtractParties( subsetParties, subsetParties_ );
    if( networkSimulationPort_ != 0 )
        networkSimulationParameters_ =
            networkSimulationParameters_.substr( 0, networkSimulationParameters_.find( ':' ) )
            + ':' + boost::lexical_cast< std::string >( networkSimulationPort_ );
    if( ! networkClientsParameters_ )
        networkClientsParameters_ = port;
    SetDispatcherProtobufLogSettings( logDispatcherProtobuf );
    SetDispatcherLogSettings( logDispatcher );
    SetLoggerPluginLogSettings( logLoggerPlugin );
    SetShieldLogSettings( logShield );
}

// -----------------------------------------------------------------------------
// Name: Config::GetNetworkSimulationParameters
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
const std::string& Config::GetNetworkSimulationParameters() const
{
    return networkSimulationParameters_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetNetworkClientsParameters
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
unsigned short Config::GetNetworkClientsParameters() const
{
    return networkClientsParameters_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetNetworkShieldParameters
// Created: MCO 2011-11-29
// -----------------------------------------------------------------------------
unsigned short Config::GetNetworkShieldParameters() const
{
    return networkShieldParameters_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetNetworkTimeout
// Created: JSR 2011-10-19
// -----------------------------------------------------------------------------
unsigned long Config::GetNetworkTimeout() const
{
    return networkTimeout_;
}

// -----------------------------------------------------------------------------
// Name: Config::UseShieldUtf8Encoding
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
bool Config::UseShieldUtf8Encoding() const
{
    return useShieldUtf8Encoding_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetKeyFramesFrequency
// Created: JSR 2010-11-02
// -----------------------------------------------------------------------------
unsigned int Config::GetKeyFramesFrequency() const
{
    return keyFramesFrequency_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetReplayFragmentsFrequency
// Created: JSR 2010-11-02
// -----------------------------------------------------------------------------
unsigned int Config::GetReplayFragmentsFrequency() const
{
    return replayFragmentsFrequency_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetTickDuration
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
unsigned int Config::GetTickDuration() const
{
    return timeStep_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetReportsClearFrequency
// Created: LDC 2013-01-23
// -----------------------------------------------------------------------------
unsigned int Config::GetReportsClearFrequency() const
{
    return reportsClearFrequency_;
}

// -----------------------------------------------------------------------------
// Name: Config::CanCreateParty
// Created: LGY 2013-02-12
// -----------------------------------------------------------------------------
bool Config::CanCreateParty( unsigned int id ) const
{
    if( !subset_ )
        return true;
    return subsetParties_.find( id ) != subsetParties_.end();
}
