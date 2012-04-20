// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"
#include "Mocks.h"

#include <host/Agent.h>
#include <host/Json.h>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using mocks::MockLog;
using mocks::MockNode;
using mocks::MockNodeController;
using mocks::MockSession;
using mocks::MockSessionController;

namespace
{
    const std::string defaultNodeString = "0123456789abcdef0123456789abcdef";
    const boost::uuids::uuid defaultNode = boost::uuids::string_generator()( defaultNodeString );
    const std::string anotherNodeString = "0123456bcde4567f0123456789abcdef";
    const boost::uuids::uuid anotherNode = boost::uuids::string_generator()( anotherNodeString );

    static boost::shared_ptr< MockNode > CreateMockNode( const std::string& id, const std::string& name )
    {
        boost::shared_ptr< MockNode > ptr = boost::make_shared< MockNode >();
        MOCK_EXPECT( ptr->GetId ).returns( boost::uuids::string_generator()( id ) );
        boost::property_tree::ptree tree;
        tree.put( "id", id );
        tree.put( "name", name );
        MOCK_EXPECT( ptr->GetProperties ).returns( tree );
        return ptr;
    }

    static boost::shared_ptr< MockSession > CreateMockSession( const boost::uuids::uuid& node, const std::string& id, const std::string& exercise, const std::string& name )
    {
        boost::shared_ptr< MockSession > ptr = boost::make_shared< MockSession >();
        MOCK_EXPECT( ptr->GetId ).returns( boost::uuids::string_generator()( id ) );
        MOCK_EXPECT( ptr->GetNode ).returns( node );
        boost::property_tree::ptree tree;
        tree.put( "id", id );
        tree.put( "node", node );
        tree.put( "name", name );
        tree.put( "exercise", exercise );
        MOCK_EXPECT( ptr->GetProperties ).returns( tree );
        return ptr;
    }

    void CheckReply( const Reply& reply, const std::string& expected, bool valid = true )
    {
        BOOST_CHECK_EQUAL( reply.valid, valid );
        BOOST_CHECK_EQUAL( reply.data, expected );
    }

    std::string CreatePrefixedUuid( int idx )
    {
        const std::string suffix = boost::lexical_cast< std::string >( idx );
        std::string uuid = "12345678-90AB-CDEF-9876-543210123456";
        uuid.resize( uuid.size() - suffix.size() );
        return uuid + suffix;
    }

    struct Fixture
    {
        Fixture()
            : agent( log, nodes, sessions )
            , node ( AddNode( defaultNodeString, "nodeName" ) )
            , mockSessions( boost::assign::list_of
                ( AddSession( defaultNode, "myExercise", "myName", 0, true ) )
                ( AddSession( anotherNode, "anExercise", "aName", 1, true ) ) )
        {
            // NOTHING
        }

        boost::shared_ptr< MockSession > AddSession( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name, int idx, bool hasNode = false )
        {
            const std::string uuid = CreatePrefixedUuid( idx );
            boost::shared_ptr< MockSession > session = CreateMockSession( node, uuid, exercise, name );
            MOCK_EXPECT( sessions.Create ).once().with( node, exercise, name ).returns( session );
            if( hasNode )
                MOCK_EXPECT( nodes.Has ).once().with( node ).returns( true );
            CheckReply( agent.CreateSession( node, exercise, name ), ToJson( session->GetProperties() ) );
            return session;
        }

        boost::shared_ptr< MockNode > AddNode( const std::string& id, const std::string& name )
        {
            boost::shared_ptr< MockNode > node = CreateMockNode( id, name );
            MOCK_EXPECT( nodes.Create ).once().with( name ).returns( node );
            CheckReply( agent.CreateNode( name ), ToJson( node->GetProperties() ) );
            return node;
        }

        bool CheckIsNodePredicate( SessionController_ABC::T_Predicate predicate, const boost::uuids::uuid& id )
        {
            BOOST_FOREACH( boost::shared_ptr< Session_ABC > ptr, mockSessions )
                BOOST_CHECK_EQUAL( predicate( *ptr ), ptr->GetNode() == id );
            return true;
        }

        MockLog log;
        MockSessionController sessions;
        MockNodeController nodes;
        Agent agent;
        boost::shared_ptr< MockNode > node;
        std::vector< boost::shared_ptr< Session_ABC > > mockSessions;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_list_nodes, Fixture )
{
    const int offset = 0, limit = 2;
    MOCK_EXPECT( nodes.List ).once().with( offset, limit ).returns( boost::assign::list_of( node ) );
    CheckReply( agent.ListNodes( offset, limit ), "[" + ToJson( node->GetProperties() ) + "]" );
}

BOOST_FIXTURE_TEST_CASE( agent_count_nodes, Fixture )
{
    MOCK_EXPECT( nodes.Count ).once().returns( 17 );
    CheckReply( agent.CountNodes(), "{\"count\":\"17\"}" );
}

BOOST_FIXTURE_TEST_CASE( agent_get_node, Fixture )
{
    MOCK_EXPECT( nodes.Get ).once().with( defaultNode ).returns( node );
    CheckReply( agent.GetNode( defaultNode ), ToJson( node->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_create_node, Fixture )
{
    MOCK_EXPECT( nodes.Create ).once().with( "zomg" ).returns( node );
    CheckReply( agent.CreateNode( "zomg" ), ToJson( node->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_node, Fixture )
{
    MOCK_EXPECT( nodes.Delete ).once().with( defaultNode ).returns( node );
    MOCK_EXPECT( sessions.List ).with( boost::bind( &Fixture::CheckIsNodePredicate, this, _1, defaultNode ), 0, mock::any ).once().returns( boost::assign::list_of( mockSessions[0] ) );
    MOCK_EXPECT( sessions.Delete ).once().with( mockSessions[0]->GetId() ).returns( mockSessions[0] );
    CheckReply( agent.DeleteNode( defaultNode ), ToJson( node->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_start_node, Fixture )
{
    MOCK_EXPECT( nodes.Start ).once().with( defaultNode ).returns( node );
    CheckReply( agent.StartNode( defaultNode ), ToJson( node->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_node, Fixture )
{
    MOCK_EXPECT( nodes.Stop ).once().with( defaultNode ).returns( node );
    CheckReply( agent.StopNode( defaultNode ), ToJson( node->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_list_sessions, Fixture )
{
    const int offset = 1, limit = 1;
    MOCK_EXPECT( sessions.List ).once().with( mock::any, offset, limit ).returns( boost::assign::list_of( mockSessions[1] ) );
    CheckReply( agent.ListSessions( defaultNode, offset, limit ), "[" + ToJson( mockSessions[1]->GetProperties() ) + "]" );
    MOCK_EXPECT( sessions.List ).once().with( mock::any, 0, 1 ).returns( boost::assign::list_of( mockSessions[0] ) );
    CheckReply( agent.ListSessions( boost::uuids::nil_uuid(), 0, 1 ), "[" + ToJson( mockSessions[0]->GetProperties() ) + "]" );
}

BOOST_FIXTURE_TEST_CASE( agent_count_sessions, Fixture )
{
    MOCK_EXPECT( sessions.Count ).once().returns( 17 );
    CheckReply( agent.CountSessions( boost::uuids::nil_uuid() ), "{\"count\":\"17\"}" );
}

BOOST_FIXTURE_TEST_CASE( agent_get_session, Fixture )
{
    MOCK_EXPECT( sessions.Get ).once().with( mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckReply( agent.GetSession( mockSessions[1]->GetId() ), ToJson( mockSessions[1]->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_cannot_create_orphan_session, Fixture )
{
    MOCK_EXPECT( nodes.Has ).once().with( defaultNode ).returns( false );
    CheckReply( agent.CreateSession( defaultNode, "exercise", "name" ), "invalid node", false );
}

BOOST_FIXTURE_TEST_CASE( agent_create_session, Fixture )
{
    MOCK_EXPECT( nodes.Has ).once().with( defaultNode ).returns( true );
    boost::shared_ptr< MockSession > session = AddSession( defaultNode, "exerciseName", "sessionName", 10000 );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_session, Fixture )
{
    MOCK_EXPECT( sessions.Delete ).once().with( mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckReply( agent.DeleteSession( mockSessions[1]->GetId() ), ToJson( mockSessions[1]->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_start_session, Fixture )
{
    MOCK_EXPECT( sessions.Start ).once().with( mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckReply( agent.StartSession( mockSessions[1]->GetId() ), ToJson( mockSessions[1]->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_session, Fixture )
{
    MOCK_EXPECT( sessions.Stop ).once().with( mockSessions[1]->GetId() ).returns( mockSessions[1] );
    CheckReply( agent.StopSession( mockSessions[1]->GetId() ), ToJson( mockSessions[1]->GetProperties() ) );
}

BOOST_FIXTURE_TEST_CASE( agent_list_exercises, Fixture )
{
    const std::vector< std::string > list = boost::assign::list_of( "a" )( "b" )( "c" );
    MOCK_EXPECT( sessions.GetExercises ).once().returns( list );
    CheckReply( agent.ListExercises( 0, 10 ), "[\"a\", \"b\", \"c\"]" );
}

BOOST_FIXTURE_TEST_CASE( agent_list_empty_exercises, Fixture )
{
    const std::vector< std::string > list = boost::assign::list_of( "a" )( "b" )( "c" );
    MOCK_EXPECT( sessions.GetExercises ).once().returns( list );
    CheckReply( agent.ListExercises( 0, INT_MAX ), "[\"a\", \"b\", \"c\"]" );
    MOCK_EXPECT( sessions.GetExercises ).once().returns( list );
    CheckReply( agent.CountExercises(), "{\"count\":\"3\"}" );
}
