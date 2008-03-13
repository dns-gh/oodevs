// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Config.h"
#include "gaming/Network.h"
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 4244 4511 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
namespace po  = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv )
{
    po::options_description desc( "Replayer options" );
    desc.add_options()
        ( "host", po::value< std::string >( &host_ ), "specify host to join" )
    ;
    AddOptions( desc );

    Parse( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::Connect
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void Config::Connect( Network& network ) const
{
    if( ! host_.empty() )
        network.Connect( host_ );
    else 
        LoadSession( network );
}

// -----------------------------------------------------------------------------
// Name: Config::LoadSession
// Created: AGE 2008-01-07
// -----------------------------------------------------------------------------
void Config::LoadSession( Network& network ) const
{
    const std::string session = GetSessionFile();
    if( session.empty() )
        return;

    try
    {
        std::string host;
        xml::xifstream xis( session );
        xis >> xml::start( "session" )
            >> xml::start( "config" )
            >> xml::start( "gaming" )
            >> xml::start( "network" )
                >> xml::attribute( "server", host );
        network.Connect( host );
    }
    catch( ... )
    {
        // NOTHING
    }
}
