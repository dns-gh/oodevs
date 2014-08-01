// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "GamingConfig.h"
#include "gaming/Network.h"
#include "clients_gui/GamingCommonConfig.h"
#include "clients_kernel/Tools.h"
#include "tools/NullFileLoaderObserver.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po  = boost::program_options;

// -----------------------------------------------------------------------------
// Name: GamingConfig constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
GamingConfig::GamingConfig( int argc, char** argv )
    : SessionConfig( tools::CreateNullFileLoaderObserver() )
    , orderFile_     ( "" )
    , networkTimeOut_( 10000 )
    , hasTimeline_( false )
    , timelineUrl_( "" )
    , timelineDebugPort_( 0 )
    , common_( new gui::GamingCommonConfig() )
{
    std::string timelineLog, cefLog;
    po::options_description desc( "Gaming options" );
    desc.add_options()
        ( "host",  po::value( &host_ ), "specify host to join" )
        ( "login", po::value( &login_ ), "specify login" )
        ( "password", po::value( &password_ ), "specify password" )
        ( "order-file", po::value( &orderFile_ ), "specify an order file to load" )
        ( "timeline-log", po::value( &timelineLog ), "timeline log file" )
        ( "cef-log", po::value( &cefLog ), "chrome embedded log file" )
        ( "legacy-timeline", po::value( &hasTimeline_ )->zero_tokens(), "enable legacy timeline" )
        ( "timeline-debug-port",  po::value( &timelineDebugPort_ ), "timeline chrome debugger port" );
    gui::AddGamingCommonOptions( desc, *common_ );

    AddOptions( desc );
    Parse( argc, argv );
    isLoginInCommandLine_ = IsSet( "login" );
    if( isLoginInCommandLine_ && login_ == "anonymous" )
        login_ = "";
    timelineLogFile_ = tools::Path::FromUTF8( timelineLog );
    cefLogFile_ = tools::Path::FromUTF8( cefLog );
    ReadSession();
}

// -----------------------------------------------------------------------------
// Name: GamingConfig destructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
GamingConfig::~GamingConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::ReadSession
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void GamingConfig::ReadSession()
{
    const tools::Path session = GetSessionFile();
    if( !session.IsEmpty() && session.Exists() )
    {
        tools::Xifstream xis( session );
        xis >> xml::start( "session" )
                >> xml::start( "config" )
                    >> xml::start( "gaming" )
                        >> xml::start( "network" )
                            >> xml::optional >> xml::attribute( "timeout", networkTimeOut_ );
        if( host_.empty() )
            xis >> xml::attribute( "server", host_ );
        xis             >> xml::end // network
                    >> xml::end; // gaming
        xis >> xml::optional >> xml::start( "timeline" )
                >> xml::attribute( "url", timelineUrl_ );
    }
    else
    {
        throw MASA_EXCEPTION( tools::translate( "Config", "Invalid session file '%1'" ).arg( QString::fromStdWString( session.ToUnicode() ) ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::Connect
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void GamingConfig::Connect( Network& network ) const
{
    network.DoConnect( host_ );
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::GetLogin
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& GamingConfig::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::GetPassword
// Created: BAX 2013-10-24
// -----------------------------------------------------------------------------
const std::string& GamingConfig::GetPassword() const
{
    return password_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::GetOrbatFile
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
tools::Path GamingConfig::GetOrderFile() const
{
    return orderFile_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::IsLoginInCommandLine
// Created: JSR 2010-05-18
// -----------------------------------------------------------------------------
bool GamingConfig::IsLoginInCommandLine() const
{
    return isLoginInCommandLine_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::GetNetworkTimeOut
// Created: JSR 2011-10-19
// -----------------------------------------------------------------------------
unsigned long GamingConfig::GetNetworkTimeOut() const
{
    return networkTimeOut_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::HasTimeline
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
bool GamingConfig::HasTimeline() const
{
    return hasTimeline_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::GetTimelineUrl
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
std::string GamingConfig::GetTimelineUrl() const
{
    return timelineUrl_;
}

// -----------------------------------------------------------------------------
// Name: GamingConfig::GetTimelineDebugPort
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
int GamingConfig::GetTimelineDebugPort() const
{
    return timelineDebugPort_;
}

tools::Path GamingConfig::GetTimelineClientLogFile() const
{
    return timelineLogFile_;
}

tools::Path GamingConfig::GetCefLogFile() const
{
    return cefLogFile_;
}

bool GamingConfig::HasMapnik() const
{
    return common_->hasMapnik;
}

uint32_t GamingConfig::GetMapnikThreads() const
{
    return common_->mapnikThreads;
}
