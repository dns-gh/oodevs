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

#pragma warning( push )
#pragma warning( disable: 4127 4244 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include "xeumeuleu/xml.h"

namespace po = boost::program_options;
using namespace dispatcher;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Config::Config()
    : networkClientsParameters_( 0 )
{
    po::options_description desc( "Dispatcher/replayer options" );
    desc.add_options()
        ( "port" , po::value( &networkClientsParameters_ ), "specify the serving port" )
    ;
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

    int port;
    xml::xifstream xisGame( GetSessionFile() );
    xisGame >> start( "session" )
                >> start( "config" )
                    >> start( "dispatcher" )
                        >> start( "network" )
                            >> attribute( "client", networkSimulationParameters_ )
                            >> attribute( "server", port )
                        >> end()
                    >> end()
                >> end()
            >> end();
    if( ! networkClientsParameters_ )
        networkClientsParameters_ = unsigned short( port );
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

