// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include <cpplog/cpplog.hpp>

#include <web/Agent_ABC.h>

#include <web/Controller.h>
#include <web/Observer_ABC.h>
#include <web/Request_ABC.h>

#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace web;

namespace
{
    const std::string defaultIdString = "0123456789abcdef0123456789abcdef";
    const Uuid defaultId = boost::uuids::string_generator()( defaultIdString );

    MOCK_BASE_CLASS( MockLog, cpplog::BaseLogger )
    {
        MOCK_METHOD( sendLogMessage, 1 );
        MockLog()
        {
            MOCK_EXPECT( sendLogMessage ).returns( true );
        }
    };

    MOCK_BASE_CLASS( MockAgent, web::Agent_ABC )
    {
        // cluster
        MOCK_METHOD( GetCluster, 0 );
        MOCK_METHOD( StartCluster, 0 );
        MOCK_METHOD( StopCluster, 0 );
        // nodes
        MOCK_METHOD( ListNodes, 2 );
        MOCK_METHOD( CountNodes, 0 );
        MOCK_METHOD( GetNode, 1 );
        MOCK_METHOD( CreateNode, 1 );
        MOCK_METHOD( DeleteNode, 1 );
        MOCK_METHOD( StartNode, 1 );
        MOCK_METHOD( StopNode, 1 );
        // install
        MOCK_METHOD( GetInstall, 1 );
        MOCK_METHOD( DeleteInstall, 2 );
        // cache
        MOCK_METHOD( UploadCache, 2 );
        MOCK_METHOD( GetCache, 1 );
        MOCK_METHOD( DeleteCache, 1 );
        MOCK_METHOD( InstallFromCache, 2 );
        // sessions
        MOCK_METHOD( ListSessions, 3 );
        MOCK_METHOD( CountSessions, 1 );
        MOCK_METHOD( GetSession, 1 );
        MOCK_METHOD( CreateSession, 3 );
        MOCK_METHOD( DeleteSession, 1 );
        MOCK_METHOD( StartSession, 1 );
        MOCK_METHOD( StopSession, 1 );
        // exercises
        MOCK_METHOD( ListExercises, 3 );
        MOCK_METHOD( CountExercises, 1 );
    };

    MOCK_BASE_CLASS( MockRequest, Request_ABC )
    {
        MOCK_METHOD( GetUri, 0 );
        MOCK_METHOD( GetParameter, 1 );
        MOCK_METHOD( GetHeader, 1 );
        MOCK_METHOD( RegisterMime, 2 );
        MOCK_METHOD( ParseMime, 0 );
    };

    const boost::xpressive::sregex httpCodeRegex = boost::xpressive::sregex::compile( "^HTTP\\/1\\.1\\s+(\\d+)\\s+.+\r\n" );
    bool CheckHttpCode( int code, const std::string& data )
    {
        boost::xpressive::smatch what;
        bool match = boost::xpressive::regex_search( data, what, httpCodeRegex );
        return match && !what[1].compare( boost::lexical_cast< std::string >( code ) );
    }

    const boost::xpressive::sregex httpHeaderRegex = boost::xpressive::sregex::compile( "^.+\r\n\r\n" );
    std::string EraseHttpHeader( const std::string& data )
    {
        return boost::xpressive::regex_replace( data, httpHeaderRegex, "" );
    }

    void CheckReply( int code, const std::string& expected, const std::string& reply )
    {
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        BOOST_CHECK_EQUAL( expected, EraseHttpHeader( reply ));
    }

    struct Fixture
    {
        Fixture()
            : controller( log, agent )
        {
            // NOTHING
        }
        void SetRequest( const std::string& method, const std::string& uri, const std::map< std::string, std::string >& parameters = std::map< std::string, std::string >() )
        {
            MOCK_EXPECT( request.GetUri ).once().returns( uri );
            typedef std::map< std::string, std::string > Pair;
            BOOST_FOREACH( const Pair::value_type& pair, parameters )
                MOCK_EXPECT( request.GetParameter ).once().with( pair.first ).returns( pair.second );
        }
        MockLog log;
        MockRequest request;
        MockAgent agent;
        Controller controller;
    };
}

BOOST_FIXTURE_TEST_CASE( controller_get_cluster, Fixture )
{
    SetRequest( "GET", "/get_cluster" );
    const std::string expected = "a json cluster";
    MOCK_EXPECT( agent.GetCluster ).once().returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_start_cluster, Fixture )
{
    SetRequest( "GET", "/start_cluster" );
    const std::string expected = "a json cluster";
    MOCK_EXPECT( agent.StartCluster ).once().returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_cluster, Fixture )
{
    SetRequest( "GET", "/stop_cluster" );
    const std::string expected = "a json cluster";
    MOCK_EXPECT( agent.StopCluster ).once().returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_nodes, Fixture )
{
    SetRequest( "GET", "/list_nodes", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::string expected = "a json list";
    MOCK_EXPECT( agent.ListNodes ).once().with( 5, 3 ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_count_nodes, Fixture )
{
    SetRequest( "GET", "/count_nodes" );
    const std::string expected = "a json number";
    MOCK_EXPECT( agent.CountNodes ).once().returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_get_node, Fixture )
{
    SetRequest( "GET", "/get_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json node";
    MOCK_EXPECT( agent.GetNode ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_create_node, Fixture )
{
    const std::string name = "node_name";
    SetRequest( "GET", "/create_node", boost::assign::map_list_of
        ( "name", name )
    );
    const std::string expected = "a json node";
    MOCK_EXPECT( agent.CreateNode ).once().with( name ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_node, Fixture )
{
    SetRequest( "GET", "/delete_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json node";
    MOCK_EXPECT( agent.DeleteNode ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_start_node, Fixture )
{
    SetRequest( "GET", "/start_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json node";
    MOCK_EXPECT( agent.StartNode ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_node, Fixture )
{
    SetRequest( "GET", "/stop_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json node";
    MOCK_EXPECT( agent.StopNode ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_sessions, Fixture )
{
    SetRequest( "GET", "/list_sessions", boost::assign::map_list_of
        ( "node",   defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::string expected = "a json list";
    MOCK_EXPECT( agent.ListSessions ).once().with( defaultId, 5, 3 ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_count_sessions, Fixture )
{
    SetRequest( "GET", "/count_sessions", boost::assign::map_list_of( "node", defaultIdString ) );
    const std::string expected = "a json number";
    MOCK_EXPECT( agent.CountSessions ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_get_session, Fixture )
{
    SetRequest( "GET", "/get_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.GetSession ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_create_session, Fixture )
{
    const std::string exercise = "exercise_name";
    const std::string name = "session_name";
    SetRequest( "GET", "/create_session", boost::assign::map_list_of
        ( "node", defaultIdString )
        ( "exercise", exercise )
        ( "name", name )
    );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.CreateSession ).once().with( defaultId, name, exercise ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_create_session_without_node_parameter_returns_bad_request, Fixture )
{
    MOCK_EXPECT( request.GetUri ).once().returns( "/create_session" );
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    CheckReply( 400, "missing node parameter", controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_session, Fixture )
{
    SetRequest( "GET", "/delete_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.DeleteSession ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_start_session, Fixture )
{
    SetRequest( "GET", "/start_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.StartSession ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_session, Fixture )
{
    SetRequest( "GET", "/stop_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.StopSession ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_exercises, Fixture )
{
    SetRequest( "GET", "/list_exercises", boost::assign::map_list_of
        ( "id", defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::string expected = "a json list";
    MOCK_EXPECT( agent.ListExercises ).once().with( defaultId, 5, 3 ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_count_exercises, Fixture )
{
    SetRequest( "GET", "/count_exercises", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json number";
    MOCK_EXPECT( agent.CountExercises ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_ids, Fixture )
{
    static const char* targets[] =
    {
        "/get_node",    "/start_node",    "/stop_node",    "/delete_node",
        "/get_session", "/start_session", "/stop_session", "/delete_session",
    };
    for( size_t i = 0; i < sizeof( targets ) / sizeof*( targets ); ++i )
    {
        SetRequest( "GET", targets[i], boost::assign::map_list_of( "id", "1" ) );
        CheckReply( 400, "invalid \"uuid\" 1", controller.DoGet( request ) );
    }
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_parameters, Fixture )
{
    SetRequest( "GET", "/list_exercises", boost::assign::map_list_of( "id", defaultIdString )( "offset", "abc" ) );
    CheckReply( 400, "invalid parameter \"offset\"=\"abc\"", controller.DoGet( request ) );
}

namespace
{
bool RegisterMime( Request_ABC::MimeHandler& dst, const Request_ABC::MimeHandler& src )
{
    dst = src;
    return true;
}
}

BOOST_FIXTURE_TEST_CASE( controller_upload_cache, Fixture )
{
    SetRequest( "POST", "/upload_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    Request_ABC::MimeHandler handler;
    MOCK_EXPECT( request.RegisterMime ).once().with( "cache", boost::bind( &RegisterMime, boost::ref( handler ), _1 ) );
    const std::string dummy = "dummy mime file";
    std::stringstream stream( dummy );
    MOCK_EXPECT( request.ParseMime ).once().calls( boost::bind( boost::apply< void >(), boost::cref( handler ), boost::ref( stream ) ) );
    const std::string expected = "dummy reply";
    MOCK_EXPECT( agent.UploadCache ).once().with( defaultId, boost::ref( stream ) ).returns( expected );
    CheckReply( 200, expected, controller.DoPost( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_get_cache, Fixture )
{
    SetRequest( "GET", "/get_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json package";
    MOCK_EXPECT( agent.GetCache ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_cache, Fixture )
{
    SetRequest( "GET", "/delete_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "a json pack";
    MOCK_EXPECT( agent.DeleteCache ).once().with( defaultId ).returns( expected );
    CheckReply( 200, expected, controller.DoGet( request ) );
}
