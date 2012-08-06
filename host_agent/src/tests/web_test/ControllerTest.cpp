// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include "cpplog/cpplog.hpp"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Agent_ABC.h"
#include "web/Controller.h"
#include "web/Observer_ABC.h"
#include "web/HttpException.h"
#include "web/Request_ABC.h"

#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <vector>

#include "MockUserController.h"

using namespace property_tree;
using namespace web;
using mocks::MockUserController;

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
        MOCK_METHOD( CreateNode, 4 );
        MOCK_METHOD( DeleteNode, 1 );
        MOCK_METHOD( StartNode, 1 );
        MOCK_METHOD( StopNode, 1 );
        MOCK_METHOD( UpdateNode, 4 );
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
        MOCK_METHOD( GetSession, 2 );
        MOCK_METHOD( CreateSession, 3 );
        MOCK_METHOD( DeleteSession, 2 );
        MOCK_METHOD( StartSession, 2 );
        MOCK_METHOD( StopSession, 2 );
        MOCK_METHOD( PauseSession, 2 );
        MOCK_METHOD( UpdateSession, 3 );
        MOCK_METHOD( ArchiveSession, 2 );
        MOCK_METHOD( RestoreSession, 2 );
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
        MOCK_METHOD( ParseForm, 0 );
        MOCK_METHOD( GetRemoteIp, 0 );
        MOCK_METHOD( GetSid, 0 );
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

    void CheckList( int code, const std::vector< Tree >& list, const std::string& reply )
    {
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        std::string json;
        BOOST_FOREACH( const Tree& it, list )
            json += ToJson( it ) + ",";
        BOOST_CHECK_EQUAL( "[" + json.substr( 0, json.size() - 1 ) + "]", EraseHttpHeader( reply ) );
    }

    void CheckPaths( int code, const std::vector< Path >& list, const std::string& reply )
    {
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        std::string json;
        BOOST_FOREACH( const Path& it, list )
        {
            std::string item = runtime::Utf8Convert( it );
            std::replace( item.begin(), item.end(), '\\', '/' );
            json += "\"" + item + "\",";
        }
        BOOST_CHECK_EQUAL( "[" + json.substr( 0, json.size()-1 ) + "]", EraseHttpHeader( reply ) );
    }

    void CheckCount( int code, size_t count, const std::string& reply )
    {
        BOOST_CHECK( CheckHttpCode( code, reply ) );
        Tree dst;
        dst.put( "count", count );
        BOOST_CHECK_EQUAL( ToJson( dst ), EraseHttpHeader( reply ) );
    }

    struct Fixture
    {
        Fixture()
            : controller( log, agent, users, false )
        {
            // NOTHING
        }
        void SetRequest( const std::string& /*method*/, const std::string& uri, const std::map< std::string, std::string >& parameters = std::map< std::string, std::string >() )
        {
            MOCK_EXPECT( request.GetUri ).once().returns( uri );
            typedef std::map< std::string, std::string > Pair;
            BOOST_FOREACH( const Pair::value_type& pair, parameters )
                MOCK_EXPECT( request.GetParameter ).once().with( pair.first ).returns( pair.second );
        }
        MockLog log;
        MockRequest request;
        MockAgent agent;
        MockUserController users;
        Controller controller;
    };
}

BOOST_FIXTURE_TEST_CASE( controller_get_cluster, Fixture )
{
    SetRequest( "GET", "/get_cluster" );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetCluster ).once().returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_start_cluster, Fixture )
{
    SetRequest( "GET", "/start_cluster" );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StartCluster ).once().returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_cluster, Fixture )
{
    SetRequest( "GET", "/stop_cluster" );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StopCluster ).once().returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_nodes, Fixture )
{
    SetRequest( "GET", "/list_nodes", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const Tree dummy = FromJson( "{\"dummy\":\"ymmud\"}" );
    const std::vector< Tree > expected = boost::assign::list_of( dummy )( dummy );
    MOCK_EXPECT( agent.ListNodes ).once().with( 5, 3 ).returns( expected );
    CheckList( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_count_nodes, Fixture )
{
    SetRequest( "GET", "/count_nodes" );
    const size_t expected = 13;
    MOCK_EXPECT( agent.CountNodes ).once().returns( expected );
    CheckCount( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_get_node, Fixture )
{
    SetRequest( "GET", "/get_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetNode ).once().with( defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_create_node, Fixture )
{
    const std::string name = "node_name";
    SetRequest( "GET", "/create_node", boost::assign::map_list_of
        ( "ident", name )
        ( "name", name )
        ( "num_sessions", "16" )
        ( "parallel_sessions", "8" )
    );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.CreateNode ).once().with( name, name, 16, 8 ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_node, Fixture )
{
    SetRequest( "GET", "/delete_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.DeleteNode ).once().with( defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_start_node, Fixture )
{
    SetRequest( "GET", "/start_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StartNode ).once().with( defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_node, Fixture )
{
    SetRequest( "GET", "/stop_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StopNode ).once().with( defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_sessions, Fixture )
{
    SetRequest( "GET", "/list_sessions", boost::assign::map_list_of
        ( "node",   defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const Tree dummy = FromJson( "{\"dummy\":\"ymmud\"}" );
    const std::vector< Tree > expected = boost::assign::list_of( dummy )( dummy );
    MOCK_EXPECT( agent.ListSessions ).once().with( defaultId, 5, 3 ).returns( expected );
    CheckList( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_empty_sessions, Fixture )
{
    SetRequest( "GET", "/list_sessions", boost::assign::map_list_of
        ( "node",   defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::vector< Tree > expected;
    MOCK_EXPECT( agent.ListSessions ).once().with( defaultId, 5, 3 ).returns( expected );
    CheckList( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_count_sessions, Fixture )
{
    SetRequest( "GET", "/count_sessions", boost::assign::map_list_of( "node", defaultIdString ) );
    MOCK_EXPECT( agent.CountSessions ).once().with( defaultId ).returns( 17 );
    CheckCount( 200, 17, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_get_session, Fixture )
{
    SetRequest( "GET", "/get_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( agent.GetSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
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
    MOCK_EXPECT( request.GetParameter ).with( mock::any ).returns( boost::none );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.CreateSession ).once().with( defaultId, mock::any, exercise ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_create_session_without_node_parameter_returns_bad_request, Fixture )
{
    MOCK_EXPECT( request.GetUri ).once().returns( "/create_session" );
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    CheckReply( 400, std::string(), controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_session, Fixture )
{
    SetRequest( "GET", "/delete_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( agent.DeleteSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_start_session, Fixture )
{
    SetRequest( "GET", "/start_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( agent.StartSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_session, Fixture )
{
    SetRequest( "GET", "/stop_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( agent.StopSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_list_exercises, Fixture )
{
    SetRequest( "GET", "/list_exercises", boost::assign::map_list_of
        ( "id", defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const Path dummy = "/a/B/c/D";
    const std::vector< Path > expected = boost::assign::list_of( dummy )( dummy );
    MOCK_EXPECT( agent.ListExercises ).once().with( defaultId, 5, 3 ).returns( expected );
    CheckPaths( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_count_exercises, Fixture )
{
    SetRequest( "GET", "/count_exercises", boost::assign::map_list_of( "id", defaultIdString ) );
    MOCK_EXPECT( agent.CountExercises ).once().with( defaultId ).returns( 13 );
    CheckCount( 200, 13, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_ids, Fixture )
{
    static const std::string targets[] =
    {
        "/get_node",    "/start_node",    "/stop_node",    "/delete_node",
        "/get_session", "/start_session", "/stop_session", "/delete_session",
    };
    for( size_t i = 0; i < sizeof( targets ) / sizeof*( targets ); ++i )
    {
        SetRequest( "GET", targets[i], boost::assign::map_list_of( "id", "1" ) );
        if( boost::algorithm::contains( targets[i], "session" ) )
            MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
        CheckReply( 400, std::string(), controller.DoGet( request ) );
    }
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_parameters, Fixture )
{
    SetRequest( "GET", "/list_exercises", boost::assign::map_list_of( "id", defaultIdString )( "offset", "abc" ) );
    CheckReply( 400, std::string(), controller.DoGet( request ) );
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
    const std::string dummy = "{\"dummy\":\"ymmud\"}";
    std::stringstream stream( dummy );
    MOCK_EXPECT( request.ParseMime ).once().calls( boost::bind( boost::apply< void >(), boost::cref( handler ), boost::ref( stream ) ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.UploadCache ).once().with( defaultId, boost::ref( stream ) ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoPost( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_get_cache, Fixture )
{
    SetRequest( "GET", "/get_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetCache ).once().with( defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_cache, Fixture )
{
    SetRequest( "GET", "/delete_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.DeleteCache ).once().with( defaultId ).returns( FromJson( expected ) );
    CheckReply( 200, expected, controller.DoGet( request ) );
}

BOOST_FIXTURE_TEST_CASE( controller_create_node_rejects_invalid_idents, Fixture )
{
    SetRequest( "GET", "/create_node", boost::assign::map_list_of( "ident", "" ) );
    CheckReply( 400, std::string(), controller.DoGet( request ) );
    SetRequest( "GET", "/create_node", boost::assign::map_list_of( "ident", "A" ) );
    CheckReply( 400, std::string(), controller.DoGet( request ) );
    SetRequest( "GET", "/create_node", boost::assign::map_list_of( "ident", "% s+*" ) );
    CheckReply( 400, std::string(), controller.DoGet( request ) );
}