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
    , useShieldUtf8Encoding_   ( true )
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

// -----------------------------------------------------------------------------
// Name: Config::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void Config::Parse( int argc, char** argv )
{
    std::string logDispatcherSizeUnit; int logDispatcherLevel = -1, logDispatcherFiles = -1, logDispatcherFileSize = -1;
    std::string logShieldSizeUnit; int logShieldFiles = -1, logShieldFileSize = -1;
    tools::SessionConfig::Parse( argc, argv );
    unsigned short port;
    xml::xifstream xis( GetSessionFile() );
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "simulation" )
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
                    >> xml::optional >> xml::start( "debug" )
                        >> xml::optional >> xml::attribute( "loglevel", logDispatcherLevel )
                        >> xml::optional >> xml::attribute( "logfiles", logDispatcherFiles )
                        >> xml::optional >> xml::attribute( "logsize", logDispatcherFileSize )
                        >> xml::optional >> xml::attribute( "sizeunit", logDispatcherSizeUnit )
                    >> xml::end
                    >> xml::start( "plugins" )
                        >> xml::optional >>xml::start( "recorder" )
                            >> xml::optional >> xml::attribute( "fragmentfreq", replayFragmentsFrequency_ )
                            >> xml::optional >> xml::attribute( "keyframesfreq", keyFramesFrequency_ )
                        >> xml::end
                        >> xml::optional >>xml::start( "shield" )
                            >> xml::attribute( "server", networkShieldParameters_ )
                            >> xml::optional >> xml::attribute( "logfiles", logShieldFiles )
                            >> xml::optional >> xml::attribute( "logsize", logShieldFileSize )
                            >> xml::optional >> xml::attribute( "sizeunit", logShieldSizeUnit )
                            >> xml::optional >> xml::attribute( "use-utf8-string-encoding", useShieldUtf8Encoding_ );
    if( networkSimulationPort_ != 0 )
        networkSimulationParameters_ =
            networkSimulationParameters_.substr( 0, networkSimulationParameters_.find( ':' ) )
            + ':' + boost::lexical_cast< std::string >( networkSimulationPort_ );
    if( ! networkClientsParameters_ )
        networkClientsParameters_ = port;
    SetDispatcherLogSettings( logDispatcherLevel, logDispatcherFiles, logDispatcherFileSize, logDispatcherSizeUnit );
    SetShieldLogSettings( logShieldFiles, logShieldFileSize, logShieldSizeUnit );
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
