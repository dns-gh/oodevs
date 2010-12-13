// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Config.h"
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
    : networkClientsParameters_( 0 )
    , networkShieldParameters_ ( 0 )
    , keyFramesFrequency_      ( 100 )
    , replayFragmentsFrequency_( 150 )
{
    po::options_description desc( "Dispatcher/replayer options" );
    desc.add_options()
        ( "port" , po::value( &networkClientsParameters_ ), "specify the serving port" );
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
    tools::SessionConfig::Parse( argc, argv );
    unsigned short port;
    xml::xifstream xis( GetSessionFile() );
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "network" )
                        >> xml::attribute( "client", networkSimulationParameters_ )
                        >> xml::attribute( "server", port )
                    >> xml::end
                    >> xml::start( "plugins" )
                        >> xml::optional >>xml::start( "recorder" )
                            >> xml::optional >> xml::attribute( "fragmentfreq", replayFragmentsFrequency_ )
                            >> xml::optional >> xml::attribute( "keyframesfreq", keyFramesFrequency_ )
                        >> xml::end
                        >> xml::optional >>xml::start( "shield" )
                            >> xml::attribute( "server", networkShieldParameters_ );
    if( ! networkClientsParameters_ )
        networkClientsParameters_ = port;
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
