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
#include "runtime/FileSystem.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Agent_ABC.h"
#include "web/Configs.h"
#include "web/Controller.h"
#include "web/Observer_ABC.h"
#include "web/Plugins.h"
#include "web/HttpException.h"
#include "web/Request_ABC.h"
#include "web/Reply_ABC.h"
#include "web/User.h"

#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_generators.hpp>

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
        MOCK_METHOD( CreateNode, 2 );
        MOCK_METHOD( DeleteNode, 1 );
        MOCK_METHOD( StartNode, 1 );
        MOCK_METHOD( StopNode, 1 );
        MOCK_METHOD( UpdateNode, 2 );
        MOCK_METHOD( IdentifyNode, 1 );
        // client
        MOCK_METHOD( GetClient, 0 );
        MOCK_METHOD( DownloadClient, 1 );
        // install
        MOCK_METHOD( GetInstall, 1 );
        MOCK_METHOD( DeleteInstall, 2 );
        MOCK_METHOD_EXT( DownloadInstall, 3, void( const web::Uuid& id, web::Chunker_ABC&, size_t ), DownloadInstallById );
        MOCK_METHOD_EXT( DownloadInstall, 5, void( const web::Uuid& id, web::Chunker_ABC&, const std::string&, const std::string&, const std::string& ), DownloadInstallByChecksum );
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
        MOCK_METHOD( DeleteUser, 2 );
        MOCK_METHOD( StartSession, 3 );
        MOCK_METHOD( StopSession, 2 );
        MOCK_METHOD( PauseSession, 2 );
        MOCK_METHOD( UpdateSession, 3 );
        MOCK_METHOD( ArchiveSession, 2 );
        MOCK_METHOD( RestoreSession, 2 );
        MOCK_METHOD( DownloadSession, 3 );
        MOCK_METHOD( ReplaySession, 2 );
        MOCK_METHOD( DownloadSessionLog, 6 );
        // exercises
        MOCK_METHOD( ListExercises, 3 );
        MOCK_METHOD( CountExercises, 1 );
        // plugins
        MOCK_METHOD( ListPlugins, 2 );
        MOCK_METHOD( CountPlugins, 0 );
        // licenses
        MOCK_METHOD( ListLicenses, 1 );
        MOCK_METHOD( UploadLicenses, 1 );
    };

    MOCK_BASE_CLASS( MockRequest, Request_ABC )
    {
        MOCK_METHOD( GetUri, 0 );
        MOCK_METHOD( GetParameter, 1 );
        MOCK_METHOD( GetHeader, 1 );
        MOCK_METHOD( RegisterMime, 2 );
        MOCK_METHOD( ParseBodyAsMime, 0 );
        MOCK_METHOD( ParseBodyAsJson, 0 );
        MOCK_METHOD( GetSid, 0 );
    };

    MOCK_BASE_CLASS( MockResponse, Reply_ABC )
    {
        MOCK_METHOD( SetHeader, 2 );
        MOCK_METHOD( SetStatus, 1 );
        MOCK_METHOD( WriteHeaders, 0 );
        MOCK_METHOD( WriteContent, 1 );
        MOCK_METHOD( Write, 2 );
    };

    bool CompareTree( const std::string& actual, const std::string& expected )
    {
        return FromJson( actual ) == FromJson( expected );
    }

    void ExpectReply( MockResponse& rpy, HttpStatus status, const std::string& data )
    {
        mock::reset( rpy );
        MOCK_EXPECT( rpy.SetStatus ).once().with( status );
        MOCK_EXPECT( rpy.SetHeader ).once().with( "Content-Length", mock::any );
        MOCK_EXPECT( rpy.SetHeader );
        MOCK_EXPECT( rpy.WriteHeaders );
        MOCK_EXPECT( rpy.WriteContent ).once().with( boost::bind( &CompareTree, _1, data ) ).returns( static_cast< int >( data.size() ) );
    }

    void ExpectList( MockResponse& rpy, HttpStatus status, const std::vector< Tree >& list )
    {
        std::string json;
        BOOST_FOREACH( const Tree& it, list )
            json += ToJson( it ) + ",";
        ExpectReply( rpy, status, "[" + json.substr( 0, json.size() - 1 ) + "]" );
    }

    void ExpectPaths( MockResponse& rpy, HttpStatus status, const std::vector< Path >& list )
    {
        std::string json;
        BOOST_FOREACH( const Path& it, list )
        {
            std::string item = runtime::Utf8( it );
            std::replace( item.begin(), item.end(), '\\', '/' );
            json += "\"" + item + "\",";
        }
        ExpectReply( rpy, status, "[" + json.substr( 0, json.size()-1 ) + "]" );
    }

    void ExpectCount( MockResponse& rpy, HttpStatus status, size_t count )
    {
        Tree dst;
        dst.put( "count", count );
        ExpectReply( rpy, status, ToJson( dst ) );
    }

    template< bool secure = false >
    struct Fixture
    {
        Fixture()
            : fs        ( log )
            , plugins   ( fs, testOptions.GetDataPath( "plugins" ).ToUTF8() )
            , controller( plugins, log, agent, users, secure )
        {
            // NOTHING
        }
        void ExpectRequest( const std::string& /*method*/, const std::string& uri, const std::map< std::string, std::string >& parameters = std::map< std::string, std::string >() )
        {
            MOCK_EXPECT( request.GetUri ).once().returns( uri );
            typedef std::map< std::string, std::string > Pair;
            BOOST_FOREACH( const Pair::value_type& pair, parameters )
                MOCK_EXPECT( request.GetParameter ).once().with( pair.first ).returns( pair.second );
        }
        MockLog log;
        runtime::FileSystem fs;
        web::Plugins plugins;
        MockRequest request;
        MockResponse reply;
        MockAgent agent;
        MockUserController users;
        Controller controller;
    };
}

BOOST_FIXTURE_TEST_CASE( controller_get_cluster, Fixture<> )
{
    ExpectRequest( "GET", "/get_cluster" );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetCluster ).once().returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_start_cluster, Fixture<> )
{
    ExpectRequest( "GET", "/start_cluster" );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StartCluster ).once().returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_cluster, Fixture<> )
{
    ExpectRequest( "GET", "/stop_cluster" );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StopCluster ).once().returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_list_nodes, Fixture<> )
{
    ExpectRequest( "GET", "/list_nodes", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const Tree dummy = FromJson( "{\"dummy\":\"ymmud\"}" );
    const std::vector< Tree > expected = boost::assign::list_of( dummy )( dummy );
    MOCK_EXPECT( agent.ListNodes ).once().with( 5, 3 ).returns( expected );
    ExpectList( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_count_nodes, Fixture<> )
{
    ExpectRequest( "GET", "/count_nodes" );
    const size_t expected = 13;
    MOCK_EXPECT( agent.CountNodes ).once().returns( expected );
    ExpectCount( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_get_node, Fixture<> )
{
    ExpectRequest( "GET", "/get_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetNode ).once().with( defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_get_node_works_on_users, Fixture< true > )
{
    ExpectRequest( "GET", "/get_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetNode ).once().with( defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    MOCK_EXPECT( request.GetSid ).once().returns( "user" );
    const std::string user = "{\"type\":\"user\",\"id\":\"42\",\"name\":\"name\",\"node\":\"" + defaultIdString + "\"}";
    MOCK_EXPECT( users.IsAuthenticated ).once().with( "user" ).returns( FromJson( user ) );
    controller.DoGet( reply, request );
}

namespace
{
bool Equal( const node::Config& actual, const node::Config& expected )
{
    if( actual.name != expected.name ) return false;
    if( actual.sessions.max_play != expected.sessions.max_play ) return false;
    if( actual.sessions.max_parallel != expected.sessions.max_parallel ) return false;
    if( actual.sessions.reset != expected.sessions.reset ) return false;
    return true;
}
}

BOOST_FIXTURE_TEST_CASE( controller_create_node, Fixture<> )
{
    node::Config cfg;
    cfg.name = "some_name";
    cfg.sessions.max_play = 16;
    cfg.sessions.max_parallel = 8;
    Tree dst;
    node::WriteConfig( dst, cfg );
    dst.put( "ident", cfg.name );
    ExpectRequest( "GET", "/create_node" );
    MOCK_EXPECT( request.ParseBodyAsJson ).once().returns( dst );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.CreateNode ).once().with( cfg.name, boost::bind( &Equal, cfg, _1 ) ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoPost( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_node, Fixture<> )
{
    ExpectRequest( "GET", "/delete_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.DeleteNode ).once().with( defaultId ).returns( FromJson( expected ) );
    MOCK_EXPECT( users.DeleteUsers ).once().with( defaultId );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_start_node, Fixture<> )
{
    ExpectRequest( "GET", "/start_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StartNode ).once().with( defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_node, Fixture<> )
{
    ExpectRequest( "GET", "/stop_node", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.StopNode ).once().with( defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_list_sessions, Fixture<> )
{
    ExpectRequest( "GET", "/list_sessions", boost::assign::map_list_of
        ( "node",   defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const Tree dummy = FromJson( "{\"dummy\":\"ymmud\"}" );
    const std::vector< Tree > expected = boost::assign::list_of( dummy )( dummy );
    MOCK_EXPECT( agent.ListSessions ).once().with( mock::any, 5, 3 ).returns( expected );
    ExpectList( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_list_empty_sessions, Fixture<> )
{
    ExpectRequest( "GET", "/list_sessions", boost::assign::map_list_of
        ( "node",   defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::vector< Tree > expected;
    MOCK_EXPECT( agent.ListSessions ).once().with( mock::any, 5, 3 ).returns( expected );
    ExpectList( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_count_sessions, Fixture<> )
{
    ExpectRequest( "GET", "/count_sessions", boost::assign::map_list_of( "node", defaultIdString ) );
    MOCK_EXPECT( agent.CountSessions ).once().with( mock::any ).returns( 17 );
    ExpectCount( reply, web::OK, 17 );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_get_session, Fixture<> )
{
    ExpectRequest( "GET", "/get_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( request.GetParameter ).once().with( "current_user" ).returns( boost::none );
    MOCK_EXPECT( agent.GetSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_create_session, Fixture<> )
{
    const std::string name = "session_name";
    const std::string exercise = "exercise_name";
    Tree dst;
    dst.put( "name", name );
    dst.put( "exercise", exercise );
    ExpectRequest( "GET", "/create_session", boost::assign::map_list_of
        ( "node", defaultIdString )
    );
    MOCK_EXPECT( request.ParseBodyAsJson ).once().returns( dst );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.CreateSession ).once().with( mock::any, mock::any, exercise ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoPost( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_create_session_without_node_parameter_returns_bad_request, Fixture<> )
{
    MOCK_EXPECT( request.GetUri ).once().returns( "/create_session" );
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    ExpectReply( reply, web::BAD_REQUEST, std::string() );
    controller.DoPost( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_session, Fixture<> )
{
    ExpectRequest( "GET", "/delete_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( agent.DeleteSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_start_session, Fixture<> )
{
    ExpectRequest( "GET", "/start_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( request.GetParameter ).once().with( "checkpoint" ).returns( "checkpoint" );
    MOCK_EXPECT( agent.StartSession ).once().with( mock::any, defaultId, "checkpoint" ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_session, Fixture<> )
{
    ExpectRequest( "GET", "/stop_session", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
    MOCK_EXPECT( agent.StopSession ).once().with( mock::any, defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_list_exercises, Fixture<> )
{
    ExpectRequest( "GET", "/list_exercises", boost::assign::map_list_of
        ( "id", defaultIdString )
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const Path dummy = "/a/B/c/D";
    const std::vector< Path > expected = boost::assign::list_of( dummy )( dummy );
    MOCK_EXPECT( agent.ListExercises ).once().with( defaultId, 5, 3 ).returns( expected );
    ExpectPaths( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_count_exercises, Fixture<> )
{
    ExpectRequest( "GET", "/count_exercises", boost::assign::map_list_of( "id", defaultIdString ) );
    MOCK_EXPECT( agent.CountExercises ).once().with( defaultId ).returns( 13 );
    ExpectCount( reply, web::OK, 13 );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_ids, Fixture<> )
{
    static const std::string targets[] =
    {
        "/get_node",    "/start_node",    "/stop_node",    "/delete_node",
        "/get_session", "/start_session", "/stop_session", "/delete_session",
    };
    for( size_t i = 0; i < sizeof( targets ) / sizeof*( targets ); ++i )
    {
        ExpectRequest( "GET", targets[i], boost::assign::map_list_of( "id", "1" ) );
        if( boost::algorithm::contains( targets[i], "session" ) )
            MOCK_EXPECT( request.GetParameter ).once().with( "node" ).returns( boost::none );
        if( targets[i] == "/start_session" )
            MOCK_EXPECT( request.GetParameter ).once().with( "checkpoint" ).returns( boost::none );
        ExpectReply( reply, web::BAD_REQUEST, std::string() );
        controller.DoGet( reply, request );
    }
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_parameters, Fixture<> )
{
    ExpectRequest( "GET", "/list_exercises", boost::assign::map_list_of( "id", defaultIdString )( "offset", "abc" ) );
    ExpectReply( reply, web::BAD_REQUEST, std::string() );
    controller.DoGet( reply, request );
}

namespace
{
bool RegisterMime( Request_ABC::MimeHandler& dst, const Request_ABC::MimeHandler& src )
{
    dst = src;
    return true;
}
}

BOOST_FIXTURE_TEST_CASE( controller_upload_cache, Fixture<> )
{
    ExpectRequest( "POST", "/upload_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    Request_ABC::MimeHandler handler;
    MOCK_EXPECT( request.RegisterMime ).once().with( "cache", boost::bind( &RegisterMime, boost::ref( handler ), _1 ) );
    const std::string dummy = "{\"dummy\":\"ymmud\"}";
    std::stringstream stream( dummy );
    StreamReader src( stream );
    MOCK_EXPECT( request.ParseBodyAsMime ).once().calls( boost::bind( boost::apply< void >(), boost::cref( handler ), boost::ref( src ) ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.UploadCache ).once().with( defaultId, mock::same( src ) ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoPost( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_get_cache, Fixture<> )
{
    ExpectRequest( "GET", "/get_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.GetCache ).once().with( defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_cache, Fixture<> )
{
    ExpectRequest( "GET", "/delete_cache", boost::assign::map_list_of( "id", defaultIdString ) );
    const std::string expected = "{\"dummy\":\"ymmud\"}";
    MOCK_EXPECT( agent.DeleteCache ).once().with( defaultId ).returns( FromJson( expected ) );
    ExpectReply( reply, web::OK, expected );
    controller.DoGet( reply, request );
}

BOOST_FIXTURE_TEST_CASE( controller_create_node_rejects_invalid_idents, Fixture<> )
{
    Tree dst;
    MOCK_EXPECT( request.ParseBodyAsJson ).once().returns( dst );
    ExpectRequest( "GET", "/create_node" );
    ExpectReply( reply, web::BAD_REQUEST, std::string() );
    controller.DoPost( reply, request );
    dst.put( "ident", "A" );
    MOCK_EXPECT( request.ParseBodyAsJson ).once().returns( dst );
    ExpectRequest( "GET", "/create_node" );
    ExpectReply( reply, web::BAD_REQUEST, std::string() );
    controller.DoPost( reply, request );
    dst.put( "ident", "% s+*" );
    MOCK_EXPECT( request.ParseBodyAsJson ).once().returns( dst );
    ExpectRequest( "GET", "/create_node" );
    ExpectReply( reply, web::BAD_REQUEST, std::string() );
    controller.DoPost( reply, request );
}
