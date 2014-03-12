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
#include "clients_kernel/Tools.h"
#include "tools/NullFileLoaderObserver.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po  = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv )
    : SessionConfig( tools::CreateNullFileLoaderObserver() )
    , orderFile_     ( "" )
    , networkTimeOut_( 10000 )
    , hasTimeline_( false )
    , timelineUrl_( "" )
    , timelineDebugPort_( 0 )
    , mapnik_( false )
{
    po::options_description desc( "Gaming options" );
    desc.add_options()
        ( "host",  po::value( &host_ ), "specify host to join" )
        ( "login", po::value( &login_ ), "specify login" )
        ( "password", po::value( &password_ ), "specify password" )
        ( "order-file", po::value( &orderFile_ ), "specify an order file to load" );
    AddOptions( desc );
    Parse( argc, argv );
    isLoginInCommandLine_ = IsSet( "login" );
    if( isLoginInCommandLine_ && login_ == "anonymous" )
        login_ = "";
    ReadSession();
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
// Name: Config::ReadSession
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void Config::ReadSession()
{
    const tools::Path session = GetSessionFile();
    if( !session.IsEmpty() && session.Exists() )
    {
        tools::Xifstream xis( session );
        xis >> xml::start( "session" )
                >> xml::start( "config" )
                    >> xml::start( "gaming" )
                        >> xml::optional >> xml::start( "mapnik" )
                            >> xml::attribute( "activate", mapnik_ )
                        >> xml::end
                        >> xml::start( "network" )
                            >> xml::optional >> xml::attribute( "timeout", networkTimeOut_ );
        if( host_.empty() )
            xis >> xml::attribute( "server", host_ );
        xis             >> xml::end // network
                    >> xml::end; // gaming
        hasTimeline_ = false;
        timelineDebugPort_ = 0;
        xis >> xml::optional >> xml::start( "timeline" )
                >> xml::attribute( "url", timelineUrl_ )
                >> xml::optional >> xml::attribute( "debug-port", timelineDebugPort_ )
                >> xml::optional >> xml::attribute( "enabled", hasTimeline_ )
                >> xml::optional >> xml::attribute( "client-log", timelineLogFile_ );
    }
    else
    {
        throw MASA_EXCEPTION( tools::translate( "Config", "Invalid session file '%1'" ).arg( QString::fromStdWString( session.ToUnicode() ) ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: Config::Connect
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void Config::Connect( Network& network ) const
{
    network.DoConnect( host_ );
}

// -----------------------------------------------------------------------------
// Name: Config::GetLogin
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& Config::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetPassword
// Created: BAX 2013-10-24
// -----------------------------------------------------------------------------
const std::string& Config::GetPassword() const
{
    return password_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetOrbatFile
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
tools::Path Config::GetOrderFile() const
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

// -----------------------------------------------------------------------------
// Name: Config::HasTimeline
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
bool Config::HasTimeline() const
{
    return hasTimeline_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetTimelineUrl
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
std::string Config::GetTimelineUrl() const
{
    return timelineUrl_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetTimelineDebugPort
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
int Config::GetTimelineDebugPort() const
{
    return timelineDebugPort_;
}

tools::Path Config::GetTimelineClientLogFile() const
{
    return timelineLogFile_;
}

bool Config::HasMapnik() const
{
    return mapnik_;
}
