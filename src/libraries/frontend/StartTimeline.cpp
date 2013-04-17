// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CommandLineTools.h"
#include "ConfigurationManipulator.h"
#include "StartTimeline.h"
#include "frontend/Profile.h"

// warning C4503: decorated name length exceeded, name was truncated
#pragma warning( disable: 4503 )
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace frontend;
namespace bpt = boost::property_tree;

namespace
{
    const std::string uuid = "3E6DCB9A-222A-4606-A0FB-D5C8A28C9DBB";

    void WriteTo( std::ostream& output, const bpt::ptree& src, bool last )
    {
        try
        {
            bpt::write_json( output, src );
            if( !last )
                output << ",";
        }
        catch( ... )
        {
            // NOTHING
        }
    }

    void WriteRunScript( const tools::Path& run, const Profile& profile,
                         int port )
    {
        tools::Ofstream output( run );
        output << "[";

        bpt::ptree login;
        login.put( "type", "USER_LOGIN" );
        login.put( "user.login.username", "" );
        login.put( "user.login.password", "" );
        WriteTo( output, login, false );

        bpt::ptree create;
        create.put( "type", "SESSION_CREATE" );
        create.put( "session.create.uuid", ::uuid );
        create.put( "session.create.name", "session" );
        WriteTo( output, create, false );

        // manual serialization for invalid bool support from boost::property_tree
        // todo use a real json library
        const QString attach = QString(
        "{"
        "    \"type\": \"SESSION_ATTACH\","
        "    \"session\": {"
        "        \"attach\": {"
        "            \"uuid\": \"%1\","
        "            \"service\": {"
        "                \"name\": \"simulation\","
        "                \"clock\": true,"
        "                \"sword\": {"
        "                    \"address\": \"localhost:%2\","
        "                    \"username\": \"%3\","
        "                    \"password\": \"%4\""
        "                }"
        "            }"
        "        }"
        "    }"
        "}," );
        output << attach.arg( QString::fromStdString( ::uuid ) )
                        .arg( port )
                        .arg( profile.GetLogin() )
                        .arg( profile.GetPassword() );

        bpt::ptree start;
        start.put( "type", "SESSION_START" );
        start.put( "session.start.uuid", ::uuid );
        WriteTo( output, start, true );
        output << "]";
    }
}

// -----------------------------------------------------------------------------
// Name: StartTimeline constructor
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
StartTimeline::StartTimeline( const tools::GeneralConfig& config,
                              const tools::Path& exercise,
                              const tools::Path& session,
                              int index,
                              const Profile& profile )
    : SpawnCommand( config, "timeline_server.exe", true )
{
    AddArgument( QString( "--port=%1" ).arg( GetPort( index, TIMELINE_PORT ) ) );
    AddArgument( QString( "--serve=%1" ) .arg( GetPort( index, TIMELINE_WEB_PORT ) ) );
    const tools::Path root = ConfigurationManipulator::GetSessionXml( config, exercise, session ).Parent();
    const tools::Path log = root / "timeline.log";
    AddArgument( QString( "--log=%1" ).arg( QString::fromStdWString( log.ToUnicode() ) ) );
    const tools::Path run = root / "timeline.run";
    AddArgument( QString( "--run=%1" ).arg( QString::fromStdWString( run.ToUnicode() ) ) );
    WriteRunScript( run, profile, GetPort( index, DISPATCHER_PORT ) );
}

// -----------------------------------------------------------------------------
// Name: StartTimeline destructor
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
StartTimeline::~StartTimeline()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartTimeline::IsHidden
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
bool StartTimeline::IsHidden() const
{
    return false;
}