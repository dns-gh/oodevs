// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/Agent.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Configs.h"
#include "web/HttpException.h"
#include "web/User.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "MockLog.h"
#include "MockNode.h"
#include "MockNodeController.h"
#include "MockSession.h"
#include "MockSessionController.h"

using namespace host;
using namespace mocks;
using namespace property_tree;
using web::Agent_ABC;

namespace
{
    const std::string defaultNodeString = "0123456789abcdef0123456789abcdef";
    const Uuid defaultNode = boost::uuids::string_generator()( defaultNodeString );
    const web::User defaultUser = web::User( 41u, "default", web::USER_TYPE_ADMINISTRATOR, defaultNode );
    const std::string anotherNodeString = "0123456bcde4567f0123456789abcdef";
    const Uuid anotherNode = boost::uuids::string_generator()( anotherNodeString );
    const web::User anotherUser = web::User( 42u, "another", web::USER_TYPE_ADMINISTRATOR, anotherNode );
    const web::User nilUser = web::User( 43u, "nil", web::USER_TYPE_ADMINISTRATOR, boost::uuids::nil_uuid() );

    static boost::shared_ptr< MockNode > CreateMockNode( const std::string& id, const std::string& name )
    {
        Tree tree;
        tree.put( "id", id );
        tree.put( "ident", name );
        tree.put( "name", name );
        tree.put( "port", 1337 );
        boost::shared_ptr< MockNode > ptr = boost::make_shared< MockNode >( boost::uuids::string_generator()( id ), tree );
        Tree props;
        props.put( "id", id );
        props.put( "name", name );
        MOCK_EXPECT( ptr->GetProperties ).returns( props );
        return ptr;
    }

    static boost::shared_ptr< MockSession > CreateMockSession( const Uuid& node, const std::string& id, const std::string& exercise, const std::string& name )
    {
        boost::shared_ptr< MockSession > ptr = boost::make_shared< MockSession >();
        MOCK_EXPECT( ptr->GetId ).returns( boost::uuids::string_generator()( id ) );
        MOCK_EXPECT( ptr->GetNode ).returns( node );
        Tree tree;
        tree.put( "id", id );
        tree.put( "owner", 42 );
        tree.put( "node", node );
        tree.put( "name", name );
        tree.put( "exercise", exercise );
        MOCK_EXPECT( ptr->GetProperties ).returns( tree );
        return ptr;
    }

    void CheckTree( const boost::function< Tree() >& operand, const Tree& expected, bool valid = true )
    {
        if( valid )
            BOOST_CHECK( operand() == expected );
        else
            BOOST_CHECK_THROW( operand(), web::HttpException );
    }

    void CheckTree( const boost::function< Tree() >& operand, const std::string& expected, bool valid = true )
    {
        CheckTree( operand, FromJson( expected ), valid );
    }

    void CheckCount( const boost::function< size_t() >& operand, const size_t expected )
    {
        BOOST_CHECK_EQUAL( operand(), expected );
    }

    void CheckTreeList( const boost::function< std::vector< Tree >() >& operand, const std::string& expected, bool valid = true )
    {
        if( valid )
        {
            std::string json;
            BOOST_FOREACH( const Tree& it, operand() )
                json += ToJson( it ) + ",";
            BOOST_CHECK_EQUAL( "[" + json.substr( 0, json.size() - 1 ) + "]", expected );
        }
        else
        {
            BOOST_CHECK_THROW( operand(), web::HttpException );
        }
    }

    void CheckPathList( const boost::function< std::vector< Path >() >& operand, const std::string& expected, bool valid = true )
    {
        if( valid )
        {
            std::string json;
            BOOST_FOREACH( const Path& it, operand() )
                json += "\"" + runtime::Utf8( it ) + "\",";
            BOOST_CHECK_EQUAL( "[" + json.substr( 0, json.size() - 1 ) + "]", expected );
        }
        else
        {
            BOOST_CHECK_THROW( operand(), web::HttpException );
        }
    }

    std::string CreatePrefixedUuid( int idx )
    {
        const std::string suffix = boost::lexical_cast< std::string >( idx );
        std::string uuid = "12345678-90AB-CDEF-9876-543210123456";
        uuid.resize( uuid.size() - suffix.size() );
        return uuid + suffix;
    }

    struct Cluster
    {
        Cluster( bool hasCluster )
            : controller( true )
        {
            ptr = CreateMockNode( CreatePrefixedUuid( 42 ), "cluster" );
            MOCK_EXPECT( controller.Count ).once().returns( size_t( 1 ) );
            MOCK_EXPECT( controller.List ).once().with( 0, 1 ).returns( boost::assign::list_of( ptr ) );
            MOCK_EXPECT( controller.Start ).once().returns( ptr );
            if( !hasCluster )
                mock::reset( controller );
        }
        MockNodeController controller;
        boost::shared_ptr< MockNode > ptr;
    };

    template< bool hasCluster = false >
    struct Fixture
    {
        Fixture()
            : cluster     ( hasCluster )
            , nodes       ( true )
            , agent       ( log, hasCluster ? &cluster.controller : 0, nodes, sessions )
            , node        ( AddNode( defaultNodeString, "nodeName" ) )
            , mockSessions( boost::assign::list_of
                ( AddSession( defaultUser, "myName", "myExercise", 0 ) )
                ( AddSession( anotherUser, "aName", "anExercise", 1 ) ) )
        {
            // NOTHING
        }

        boost::shared_ptr< MockSession > AddSession( const web::User& user, const std::string& name, const std::string& exercise, int idx )
        {
            const std::string uuid = CreatePrefixedUuid( idx );
            boost::shared_ptr< MockSession > session = CreateMockSession( user.node, uuid, exercise, name );
            MOCK_EXPECT( sessions.Create ).once().with( mock::same( user ), mock::any, exercise ).returns( session );
            web::session::Config cfg;
            cfg.name = name;
            CheckTree( boost::bind( &Agent_ABC::CreateSession, &agent, boost::cref( user ), cfg, exercise ), ToJson( session->GetProperties() ) );
            return session;
        }

        boost::shared_ptr< MockNode > AddNode( const std::string& id, const std::string& name )
        {
            boost::shared_ptr< MockNode > node = CreateMockNode( id, name );
            web::node::Config cfg;
            MOCK_EXPECT( nodes.Create ).once().with( cfg.name, mock::same( cfg ) ).returns( node );
            CheckTree( boost::bind( &Agent_ABC::CreateNode, &agent, cfg.name, boost::cref( cfg ) ), ToJson( node->GetProperties() ) );
            return node;
        }

        bool CheckIsNodePredicate( SessionController_ABC::T_Predicate predicate, const Uuid& id )
        {
            BOOST_FOREACH( boost::shared_ptr< Session_ABC > ptr, mockSessions )
                BOOST_CHECK_EQUAL( predicate( *ptr ), ptr->GetNode() == id );
            return true;
        }

        MockLog log;
        MockSessionController sessions;
        Cluster cluster;
        MockNodeController nodes;
        Agent agent;
        boost::shared_ptr< MockNode > node;
        std::vector< boost::shared_ptr< MockSession > > mockSessions;
    };


    bool CheckSessionReloadPredicate( MockNodeController& nodes, SessionController_ABC::T_Predicate predicate )
    {
        boost::shared_ptr< MockSession > session = CreateMockSession( defaultNode, anotherNodeString, "a", "b" );
        MOCK_EXPECT( nodes.Has ).once().with( defaultNode ).returns( false );
        if( predicate( *session ) )
            return false;
        MOCK_EXPECT( nodes.Has ).once().with( defaultNode ).returns( true );
        return predicate( *session );
    }
}

BOOST_AUTO_TEST_CASE( agent_reloads )
{
    MockLog log;
    MockNodeController nodes( true );
    MockSessionController sessions;
    mock::reset( sessions );
    MOCK_EXPECT( sessions.Reload ).once().with( boost::bind( &CheckSessionReloadPredicate, boost::ref( nodes ), _1 ) );
    Agent agent( log, 0, nodes, sessions );
}

BOOST_FIXTURE_TEST_CASE( agent_get_cluster, Fixture< true > )
{
    MOCK_EXPECT( cluster.controller.Get ).once().returns( cluster.ptr );
    CheckTree( boost::bind( &Agent_ABC::GetCluster, &agent ), "{\"id\":\"12345678-90AB-CDEF-9876-543210123442\",\"name\":\"cluster\"}" );
}

BOOST_FIXTURE_TEST_CASE( agent_start_cluster, Fixture< true > )
{
    MOCK_EXPECT( cluster.controller.Start ).once().returns( cluster.ptr );
    CheckTree( boost::bind( &Agent_ABC::StartCluster, &agent ), "{\"id\":\"12345678-90AB-CDEF-9876-543210123442\",\"name\":\"cluster\"}" );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_cluster, Fixture< true > )
{
    MOCK_EXPECT( cluster.controller.Stop ).once().returns( cluster.ptr );
    CheckTree( boost::bind( &Agent_ABC::StopCluster, &agent ), "{\"id\":\"12345678-90AB-CDEF-9876-543210123442\",\"name\":\"cluster\"}" );
}

BOOST_FIXTURE_TEST_CASE( agent_list_nodes, Fixture<> )
{
    const int offset = 0, limit = 2;
    MOCK_EXPECT( nodes.List ).once().with( offset, limit ).returns( boost::assign::list_of( node ) );
    CheckTreeList( boost::bind( &Agent_ABC::ListNodes, &agent, offset, limit ), "[" + ToJson( node->GetProperties() ) + "]" );
}

BOOST_FIXTURE_TEST_CASE( agent_count_nodes, Fixture<> )
{
    MOCK_EXPECT( nodes.Count ).once().returns( 17 );
    CheckCount( boost::bind( &Agent_ABC::CountNodes, &agent ), 17 );
}

BOOST_FIXTURE_TEST_CASE( agent_get_node, Fixture<> )
{
    MOCK_EXPECT( nodes.Get ).once().with( defaultNode ).returns( node );
    CheckTree( boost::bind( &Agent_ABC::GetNode, &agent, defaultNode ), node->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_create_node, Fixture<> )
{
    web::node::Config cfg;
    MOCK_EXPECT( nodes.Create ).once().with( cfg.name, mock::same( cfg ) ).returns( node );
    CheckTree( boost::bind( &Agent_ABC::CreateNode, &agent, cfg.name, boost::cref( cfg ) ), node->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_node, Fixture<> )
{
    MOCK_EXPECT( nodes.Delete ).once().with( defaultNode ).returns( node );
    MOCK_EXPECT( sessions.List ).with( boost::bind( &Fixture::CheckIsNodePredicate, this, _1, defaultNode ), 0, mock::any ).once().returns( boost::assign::list_of( mockSessions[0] ) );
    MOCK_EXPECT( sessions.Delete ).once().with( defaultNode, mockSessions[0]->GetId() ).returns( mockSessions[0] );
    CheckTree( boost::bind( &Agent_ABC::DeleteNode, &agent, defaultNode ), node->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_start_node, Fixture<> )
{
    MOCK_EXPECT( nodes.Start ).once().with( defaultNode ).returns( node );
    CheckTree( boost::bind( &Agent_ABC::StartNode, &agent, defaultNode ), node->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_node, Fixture<> )
{
    MOCK_EXPECT( nodes.Stop ).once().with( defaultNode ).returns( node );
    CheckTree( boost::bind( &Agent_ABC::StopNode, &agent, defaultNode ), node->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_list_sessions, Fixture<> )
{
    const int offset = 1, limit = 1;
    MOCK_EXPECT( sessions.List ).once().with( mock::any, offset, limit ).returns( boost::assign::list_of( mockSessions[1] ) );
    CheckTreeList( boost::bind( &Agent_ABC::ListSessions, &agent, defaultUser, offset, limit ), "[" + ToJson( mockSessions[1]->GetProperties() ) + "]" );
    MOCK_EXPECT( sessions.List ).once().with( mock::any, 0, 1 ).returns( boost::assign::list_of( mockSessions[0] ) );
    CheckTreeList( boost::bind( &Agent_ABC::ListSessions, &agent, nilUser, 0, 1 ), "[" + ToJson( mockSessions[0]->GetProperties() ) + "]" );
}

BOOST_FIXTURE_TEST_CASE( agent_count_sessions, Fixture<> )
{
    MOCK_EXPECT( sessions.Count ).once().returns( 17 );
    CheckCount( boost::bind( &Agent_ABC::CountSessions, &agent, nilUser ), 17 );
}

BOOST_FIXTURE_TEST_CASE( agent_get_session, Fixture<> )
{
    MOCK_EXPECT( sessions.Get ).once().with( anotherNode, mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckTree( boost::bind( &Agent_ABC::GetSession, &agent, boost::cref( anotherUser ), mockSessions[1]->GetId() ), mockSessions[1]->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_cannot_create_orphan_session, Fixture<> )
{
    MOCK_EXPECT( sessions.Create ).once().returns( boost::shared_ptr< MockSession >() );
    web::session::Config cfg;
    cfg.name = "name";
    CheckTree( boost::bind( &Agent_ABC::CreateSession, &agent, boost::cref( defaultUser ), cfg, "exercise" ), Tree(), false );
}

BOOST_FIXTURE_TEST_CASE( agent_create_session, Fixture<> )
{
    boost::shared_ptr< MockSession > session = AddSession( boost::cref( defaultUser ), "sessionName", "exerciseName", 10000 );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_session, Fixture<> )
{
    MOCK_EXPECT( sessions.Delete ).once().with( anotherNode, mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckTree( boost::bind( &Agent_ABC::DeleteSession, &agent, boost::cref( anotherUser ), mockSessions[1]->GetId() ), mockSessions[1]->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_start_session, Fixture<> )
{
   MOCK_EXPECT( sessions.Start ).once().with( mock::same( anotherUser ), mockSessions[1]->GetId(), mock::any ).returns( mockSessions[1] );
   CheckTree( boost::bind( &Agent_ABC::StartSession, &agent, boost::cref( anotherUser ), mockSessions[1]->GetId(), std::string() ), mockSessions[1]->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_session, Fixture<> )
{
    MOCK_EXPECT( sessions.Stop ).once().with( mock::same( anotherUser ), mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckTree( boost::bind( &Agent_ABC::StopSession, &agent, boost::cref( anotherUser ), mockSessions[1]->GetId() ), mockSessions[1]->GetProperties() );
}

BOOST_FIXTURE_TEST_CASE( agent_list_exercises, Fixture<> )
{
    const std::vector< Path > list = boost::assign::list_of( "a" )( "b" )( "c" );
    MOCK_EXPECT( nodes.GetExercises ).once().returns( list );
    CheckPathList( boost::bind( &Agent_ABC::ListExercises, &agent, defaultNode, 0, 10 ), "[\"a\",\"b\",\"c\"]" );
}

BOOST_FIXTURE_TEST_CASE( agent_count_exercises, Fixture<> )
{
    MOCK_EXPECT( nodes.CountExercises ).once().returns( 3 );
    CheckCount( boost::bind( &Agent_ABC::CountExercises, &agent, defaultNode ), 3 );
}

BOOST_FIXTURE_TEST_CASE( agent_upload_cache, Fixture<> )
{
    Tree tree;
    tree.put( "some", "data" );
    NilReader src;
    MOCK_EXPECT( nodes.UploadCache ).once().with( defaultNode, mock::same( src ) ).returns( tree );
    CheckTree( boost::bind( &Agent_ABC::UploadCache, &agent, defaultNode, boost::ref( src ) ), tree );
}

BOOST_FIXTURE_TEST_CASE( agent_get_cache, Fixture<> )
{
    Tree tree;
    tree.put( "some", "data" );
    MOCK_EXPECT( nodes.GetCache ).once().with( defaultNode ).returns( tree );
    CheckTree( boost::bind( &Agent_ABC::GetCache, &agent, defaultNode ), tree );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_cache, Fixture<> )
{
    Tree tree;
    tree.put( "some", "data" );
    MOCK_EXPECT( nodes.DeleteCache ).once().with( defaultNode ).returns( tree );
    CheckTree( boost::bind( &Agent_ABC::DeleteCache, &agent, defaultNode ), tree );
}
