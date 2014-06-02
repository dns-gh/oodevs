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

    void WriteRunScript( const tools::Path& run, int port )
    {
        tools::Ofstream output( run );
        output << "[";

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
        "                    \"address\": \"localhost:%2\""
        "                }"
        "            }"
        "        }"
        "    }"
        "}," );
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
                              const tools::Path& session )
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
    const auto debug = "session/config/timeline/@debug";
    if( xpath.HasNode( debug ) )
    {
        AddArgument( "-debug" );
        AddArgument( "www", xpath.GetValue< std::string >( debug ) );
    }
    WriteRunScript( run, boost::lexical_cast< int >( dispatcher ) );
}

// -----------------------------------------------------------------------------
// Name: StartTimeline destructor
// Created: BAX 2013-04-16
// -----------------------------------------------------------------------------
StartTimeline::~StartTimeline()
{
    // NOTHING
}
