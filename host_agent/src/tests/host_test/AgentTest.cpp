// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "host_test.h"

#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>

#include <host/Agent.h>
#include <host/NodeFactory_ABC.h>
#include <host/Node_ABC.h>
#include <host/SessionFactory_ABC.h>
#include <host/Session_ABC.h>

#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace runtime;
using namespace host;

namespace
{
    const std::string default_node_string = "0123456789abcdef0123456789abcdef";
    const boost::uuids::uuid default_node = boost::uuids::string_generator()( default_node_string );

    MOCK_BASE_CLASS( MockNodeFactory, NodeFactory_ABC )
    {
        MOCK_METHOD( Create, 1 );
        MOCK_METHOD( Reload, 0 );
        MockNodeFactory()
        {
            MOCK_EXPECT( this->Reload ).once().returns( NodeFactory_ABC::T_Nodes() );
        }
    };

    MOCK_BASE_CLASS( MockNode, Node_ABC )
    {
        MOCK_METHOD( GetTag, 0 );
        MOCK_METHOD( ToJson, 0 );
        MOCK_METHOD( Start, 0 );
        MOCK_METHOD( Stop, 0 );
    };

    MOCK_BASE_CLASS( MockSessionFactory, SessionFactory_ABC )
    {
        MOCK_METHOD( Create, 3 );
        MOCK_METHOD( Reload, 0 );
        MOCK_METHOD( GetExercises, 0 );
        MockSessionFactory()
        {
            MOCK_EXPECT( this->Reload ).once().returns( SessionFactory_ABC::T_Sessions() );
        }
    };

    MOCK_BASE_CLASS( MockSession, Session_ABC )
    {
        MOCK_METHOD( GetTag, 0 );
        MOCK_METHOD( GetNode, 0 );
        MOCK_METHOD( ToJson, 0 );
        MOCK_METHOD( Start, 0 );
        MOCK_METHOD( Stop, 0 );
    };

    static boost::shared_ptr< MockNode > CreateMockNode( const std::string& id, const std::string& name )
    {
        boost::shared_ptr< MockNode > ptr = boost::make_shared< MockNode >();
        MOCK_EXPECT( ptr->GetTag ).returns( boost::uuids::string_generator()( id ) );
        MOCK_EXPECT( ptr->ToJson ).returns( id + name );
        return ptr;
    }

    static boost::shared_ptr< MockSession > CreateMockSession( const boost::uuids::uuid& node, const std::string& id, const std::string& exercise, const std::string& name )
    {
        boost::shared_ptr< MockSession > ptr = boost::make_shared< MockSession >();
        MOCK_EXPECT( ptr->GetTag ).returns( boost::uuids::string_generator()( id ) );
        MOCK_EXPECT( ptr->GetNode ).returns( node );
        MOCK_EXPECT( ptr->ToJson ).returns( id + exercise + name );
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
            : agent( nodes, sessions )
        {
            // NOTHING
        }

        boost::shared_ptr< MockSession > AddSession( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name, int idx )
        {
            const std::string uuid = CreatePrefixedUuid( idx );
            boost::shared_ptr< MockSession > session = CreateMockSession( node, uuid, exercise, name );
            MOCK_EXPECT( sessions.Create ).once().with( node, exercise, name ).returns( session );
            MOCK_EXPECT( session->Start ).once();
            CheckReply( agent.CreateSession( node, exercise, name ), session->ToJson() );
            return session;
        }

        boost::shared_ptr< MockNode > AddNode( const std::string& id, const std::string& name )
        {
            boost::shared_ptr< MockNode > node = CreateMockNode( id, name );
            MOCK_EXPECT( nodes.Create ).once().with( name ).returns( node );
            MOCK_EXPECT( node->Start ).once();
            CheckReply( agent.CreateNode( name ), node->ToJson() );
            return node;
        }

        MockSessionFactory sessions;
        MockNodeFactory nodes;
        Agent agent;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_creates_node, Fixture )
{
    boost::shared_ptr< MockNode > node = AddNode( default_node_string, "node_name" );
    CheckReply( agent.ListNodes( 0, 2 ), "[" + node->ToJson() + "]" );
    CheckReply( agent.CountNodes(), "{ \"count\" : 1 }" );
    CheckReply( agent.GetNode( node->GetTag() ), node->ToJson() );
}

BOOST_FIXTURE_TEST_CASE( agent_deletes_node, Fixture )
{
    boost::shared_ptr< MockNode > node = AddNode( default_node_string, "node_name" );
    MOCK_EXPECT( node->Stop ).once();
    CheckReply( agent.DeleteNode( node->GetTag() ), node->ToJson() );
    CheckReply( agent.CountNodes(), "{ \"count\" : 0 }" );
    CheckReply( agent.ListNodes( 0, 10 ), "[]" );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_start_nodes, Fixture )
{
    boost::shared_ptr< MockNode > node = AddNode( default_node_string, "node_name" );
    MOCK_EXPECT( node->Stop ).once();
    CheckReply( agent.StopNode( node->GetTag() ), node->ToJson() );
    MOCK_EXPECT( node->Start ).once();
    CheckReply( agent.StartNode( node->GetTag() ), node->ToJson() );
}

BOOST_FIXTURE_TEST_CASE( agent_cannot_create_orphan_session, Fixture )
{
    CheckReply( agent.CreateSession( default_node, "exercise", "name" ), "invalid node", false );
}

BOOST_FIXTURE_TEST_CASE( agent_creates_session, Fixture )
{
    AddNode( default_node_string, "node_name" );
    boost::shared_ptr< MockSession > session = AddSession( default_node, "exercise_name", "session_name", 10000 );
    CheckReply( agent.ListSessions( default_node, 0, 2 ), "[" + session->ToJson() + "]" );
    CheckReply( agent.CountSessions( default_node ), "{ \"count\" : 1 }" );
    CheckReply( agent.GetSession( session->GetTag() ), session->ToJson() );
}

BOOST_FIXTURE_TEST_CASE( agent_clips_list_parameters, Fixture )
{
    AddNode( default_node_string, "node_name" );
    std::vector< boost::shared_ptr< MockSession > > list;
    for( int i = 0; i < 8; ++i )
        list.push_back( AddSession( default_node, "exercise_name", "session_name", i ) );
    CheckReply( agent.CountSessions( default_node ), "{ \"count\" : 8 }" );
    CheckReply( agent.ListSessions( default_node, -1,  1 ), "[" + list.front()->ToJson() + "]" );
    CheckReply( agent.ListSessions( default_node,  8,  1 ), "[]" );
    CheckReply( agent.ListSessions( default_node,  0, -1 ), "[]" );
    CheckReply( agent.ListSessions( default_node,  7,  2 ), "[" + list.back()->ToJson() + "]" );
}

BOOST_FIXTURE_TEST_CASE( agent_deletes_session, Fixture )
{
    AddNode( default_node_string, "node_name" );
    boost::shared_ptr< MockSession > session = AddSession( default_node, "exercise_name", "session_name", 10000 );
    MOCK_EXPECT( session->Stop ).once();
    CheckReply( agent.DeleteSession( session->GetTag() ), session->ToJson() );
    CheckReply( agent.CountSessions( default_node ), "{ \"count\" : 0 }" );
    CheckReply( agent.ListSessions( default_node, 0, 10 ), "[]" );
}

BOOST_FIXTURE_TEST_CASE( agent_stop_start_sessions, Fixture )
{
    AddNode( default_node_string, "node_name" );
    boost::shared_ptr< MockSession > session = AddSession( default_node, "exercise_name", "session_name", 10000 );
    MOCK_EXPECT( session->Stop ).once();
    CheckReply( agent.StopSession( session->GetTag() ), session->ToJson() );
    MOCK_EXPECT( session->Start ).once();
    CheckReply( agent.StartSession( session->GetTag() ), session->ToJson() );
}

BOOST_FIXTURE_TEST_CASE( agent_manages_sessions_per_node, Fixture )
{
    boost::shared_ptr< MockNode > node = AddNode( default_node_string, "node_name" );
    const boost::uuids::uuid another_node = AddNode( "12345678-90AB-CDEF-9876-543210123456", "another_node" )->GetTag();

    std::vector< boost::shared_ptr< MockSession > > list;
    list.push_back( AddSession( default_node, "exercise_name", "session_name", 0 ) );
    list.push_back( AddSession( another_node, "another_exercise", "another_name", 1 ) );

    CheckReply( agent.CountSessions( boost::uuids::nil_uuid() ), "{ \"count\" : 2 }" );
    CheckReply( agent.CountSessions( default_node ), "{ \"count\" : 1 }" );
    CheckReply( agent.CountSessions( another_node ), "{ \"count\" : 1 }" );
    CheckReply( agent.ListSessions( boost::uuids::nil_uuid(), 0,  10 ), "[" + list.front()->ToJson() + ", " + list.back()->ToJson() + "]" );
    CheckReply( agent.ListSessions( default_node, 0,  10 ), "[" + list.front()->ToJson() + "]" );
    CheckReply( agent.ListSessions( another_node, 0,  10 ), "[" + list.back()->ToJson() + "]" );

    MOCK_EXPECT( node->Stop ).once();
    CheckReply( agent.DeleteNode( default_node ), node->ToJson() );
    CheckReply( agent.CountSessions( boost::uuids::nil_uuid() ), "{ \"count\" : 1 }" );
    CheckReply( agent.CountSessions( default_node ), "{ \"count\" : 0 }" );
    CheckReply( agent.CountSessions( another_node ), "{ \"count\" : 1 }" );
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
    CheckReply( agent.CountExercises(), "{ \"count\" : 3 }" );
}
