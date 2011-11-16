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
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po  = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv, tools::RealFileLoaderObserver_ABC& observer )
    : SessionConfig( observer )
    , orderFile_     ( "" )
    , networkTimeOut_( 10000 )
{
    po::options_description desc( "Gaming options" );
    desc.add_options()
        ( "host",  po::value< std::string >( &host_ ), "specify host to join" )
        ( "login", po::value< std::string >( &login_ ), "specify login" )
        ( "order-file", po::value< std::string >( &orderFile_ ), "specify an order file to load" );
    AddOptions( desc );
    Parse( argc, argv );
    isLoginInCommandLine_ = IsSet( "login" );
    if( isLoginInCommandLine_ && login_ == "anonymous" )
        login_ = "";
    const std::string session = GetSessionFile();
    if( !session.empty() )
    {
        xml::xifstream xis( session );
        xis >> xml::start( "session" )
                >> xml::start( "config" )
                    >> xml::start( "gaming" )
                        >> xml::start( "network" )
                            >> xml::optional >> xml::attribute( "timeout", networkTimeOut_ );
    }
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
        network.DoConnect( host_ );
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
        network.DoConnect( host );
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: Config::GetLogin
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
std::string Config::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetOrbatFile
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
std::string Config::GetOrderFile() const
{
    return orderFile_;
}

// -----------------------------------------------------------------------------
// Name: Config::IsLoginInCommandLine
// Created: JSR 2010-05-18
// -----------------------------------------------------------------------------
bool Config::IsLoginInCommandLine() const
{
    return isLoginInCommandLine_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetNetworkTimeOut
// Created: JSR 2011-10-19
// -----------------------------------------------------------------------------
unsigned long Config::GetNetworkTimeOut() const
{
    return networkTimeOut_;
}
