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
#include "tools/FileWrapper.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/XmlStreamOperators.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::Config()
    : SessionConfig( tools::CreateNullFileLoaderObserver() )
    , networkTimeout_          ( 10000 )
    , keyFramesFrequency_      ( 100 )
    , replayFragmentsFrequency_( 150 )
    , timeStep_                ( 0 )
    , reportsClearFrequency_   ( 100 )
{
    po::options_description desc( "Dispatcher/replayer options" );
    desc.add_options()
        ( "simulation-address", po::value( &networkSimulationParameters_ ), "specify the simulation server address (ip:port)" )
        ( "dispatcher-address", po::value( &networkClientsParameters_ )   , "specify the dispatcher server address (ip:port)" );
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
    LogSettingsData logDispatcherProtobuf, logDispatcher, logLoggerPlugin;
    tools::SessionConfig::Parse( argc, argv );
    std::string simulationAddress;
    std::string dispatcherAddress;
    std::string subsetParties;
    tools::Xifstream xis( GetSessionFile() );
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
                        >> xml::attribute( "client", simulationAddress )
                        >> xml::attribute( "server", dispatcherAddress )
                        >> xml::optional >> xml::attribute( "timeout", networkTimeout_ )
                    >> xml::end
                    >> xml::optional >> xml::content( "log", logDispatcher )
                    >> xml::optional >> xml::content( "messages", logLoggerPlugin )
                    >> xml::optional >> xml::content( "debug", logDispatcherProtobuf )
                    >> xml::start( "plugins" )
                        >> xml::optional >>xml::start( "recorder" )
                            >> xml::optional >> xml::attribute( "fragmentfreq", replayFragmentsFrequency_ )
                            >> xml::optional >> xml::attribute( "keyframesfreq", keyFramesFrequency_ )
                        >> xml::end
                    >> xml::end
                    >> xml::optional >> xml::start( "reports" )
                        >> xml::attribute( "frequency", reportsClearFrequency_ )
                    >> xml::end;
    if( subset_ )
        ExtractParties( subsetParties );
    if( networkSimulationParameters_.empty() )
        networkSimulationParameters_ = simulationAddress;
    if( networkClientsParameters_.empty() )
        networkClientsParameters_ = dispatcherAddress;
    logDispatcherProtobuf.level_ = LogSettings::elogLevel_all;
    SetDispatcherProtobufLogSettings( logDispatcherProtobuf );
    SetDispatcherLogSettings( logDispatcher );
    SetLoggerPluginLogSettings( logLoggerPlugin );
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
const std::string& Config::GetNetworkClientsParameters() const
{
    return networkClientsParameters_;
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
