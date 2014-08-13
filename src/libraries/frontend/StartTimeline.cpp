// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
// warning C4503: decorated name length exceeded, name was truncated
#pragma warning( disable: 4503 )
#include "ConfigurationManipulator.h"
#include "StartTimeline.h"
#include "DebugConfig.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/regex.hpp>

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

    void WriteRunScript( const tools::Path& run, int port, bool autostart )
    {
        tools::Ofstream output( run );
        output << "[";

        // manual serialization for invalid bool support from boost::property_tree
        // todo use a real json library
        const QString create = QString(
        "{\n"
        "    \"type\": \"SESSION_CREATE\",\n"
        "    \"session\":\n"
        "    {\n"
        "        \"create\":\n"
        "        {\n"
        "            \"uuid\": \"%1\",\n"
        "            \"name\": \"session\",\n"
        "            \"autostart\": %2\n"
        "        }\n"
        "    }\n"
        "},\n" );
        output << create.arg( QString::fromStdString( ::uuid ) )
                        .arg( autostart ? "true" : "false" );
        const QString attach = QString(
        "{\n"
        "    \"type\": \"SESSION_ATTACH\",\n"
        "    \"session\": {\n"
        "        \"attach\": {\n"
        "            \"uuid\": \"%1\",\n"
        "            \"service\": {\n"
        "                \"name\": \"simulation\",\n"
        "                \"clock\": true,\n"
        "                \"sword\": {\n"
        "                    \"address\": \"localhost:%2\"\n"
        "                }\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "},\n" );
        output << attach.arg( QString::fromStdString( ::uuid ) )
                        .arg( port );

        bpt::ptree start;
        start.put( "type", "SESSION_START" );
        start.put( "session.start.uuid", ::uuid );
        WriteTo( output, start, true );
        output << "]";
    }

    std::string GetPort( const std::string& host )
    {
        static const auto re = boost::regex( "^.+:" );
        return boost::regex_replace( host, re, "" );
    }
}

// -----------------------------------------------------------------------------
// Name: StartTimeline constructor
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
StartTimeline::StartTimeline( const tools::GeneralConfig& config,
                              const tools::Path& exercise,
                              const tools::Path& session,
                              const frontend::DebugConfig& debug )
    : SpawnCommand( config, "timeline_server.exe", "timeline_server" )
{
    ConfigurationManipulator xpath( config, exercise, session );
    const auto dispatcher = GetPort( xpath.GetValue< std::string >( "session/config/dispatcher/network/@server" ) );
    const auto timeline = GetPort( xpath.GetValue< std::string >( "session/config/timeline/@url" ) );
    AddArgument( "port", timeline );
    const tools::Path root = ConfigurationManipulator::GetSessionXml( config, exercise, session ).Parent();
    const tools::Path log = root / "timeline.log";
    AddArgument( "log", log.ToUTF8() );
    const tools::Path run = root / "timeline.run";
    AddArgument( "run", run.ToUTF8() );
    if( !debug.timeline.debugWwwDir.IsEmpty() )
    {
        AddArgument( "-debug" );
        AddArgument( "www", debug.timeline.debugWwwDir.ToUTF8() );
    }
    WriteRunScript( run, boost::lexical_cast< int >( dispatcher ), debug.timeline.autostart );
}

// -----------------------------------------------------------------------------
// Name: StartTimeline destructor
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
StartTimeline::~StartTimeline()
{
    // NOTHING
}
