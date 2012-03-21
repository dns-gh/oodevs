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
    MOCK_BASE_CLASS( MockSessionFactory, SessionFactory_ABC )
    {
        MOCK_METHOD( Create, 2 );
        MOCK_METHOD( Reload, 0 );
    };

    MOCK_BASE_CLASS( MockSession, Session_ABC )
    {
        MOCK_METHOD( GetTag, 0 );
        MOCK_METHOD( ToJson, 0 );
        MOCK_METHOD( Start, 0 );
        MOCK_METHOD( Stop, 0 );
    };

    static boost::shared_ptr< MockSession > CreateMockSession( const std::string& tag, const std::string& exercise, const std::string& name )
    {
        boost::shared_ptr< MockSession > ptr = boost::make_shared< MockSession >();
        MOCK_EXPECT( ptr->GetTag ).returns( boost::uuids::string_generator()( tag ) );
        MOCK_EXPECT( ptr->ToJson ).returns( tag + exercise + name );
        return ptr;
    }

    void CheckReply( const Reply& reply, const std::string& expected, bool valid = true )
    {
        BOOST_CHECK_EQUAL( reply.valid, valid );
        BOOST_CHECK_EQUAL( reply.data, expected );
    }

    struct Fixture
    {
        Fixture()
            : agent( factory )
        {
            // NOTHING
        }
        boost::shared_ptr< MockSession > AddSession( const std::string& exercise, const std::string& name, int idx )
        {
            const std::string suffix = boost::lexical_cast< std::string >( idx );
            std::string uuid = "12345678-90AB-CDEF-9876-543210123456";
            uuid.resize( uuid.size() - suffix.size() );
            uuid += suffix;
            boost::shared_ptr< MockSession > session = CreateMockSession( uuid, exercise, name );
            MOCK_EXPECT( factory.Create ).once().with( exercise, name ).returns( session );
            MOCK_EXPECT( session->Start ).once();
            CheckReply( agent.CreateSession( exercise, name ), session->ToJson() );
            return session;
        }
        MockSessionFactory factory;
        Agent agent;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_creates_session, Fixture )
{
    boost::shared_ptr< MockSession > session = AddSession( "exercise_name", "session_name", 10000 );
    CheckReply( agent.ListSessions( 0, 2 ), "[" + session->ToJson() + "]" );
    CheckReply( agent.CountSessions(), "{ \"count\" : 1 }" );
    CheckReply( agent.GetSession( session->GetTag() ), session->ToJson() );
}

BOOST_FIXTURE_TEST_CASE( agent_clips_list_parameters, Fixture )
{
    std::vector< boost::shared_ptr< MockSession > > list;
    for( int i = 0; i < 8; ++i )
        list.push_back( AddSession( "exercise_name", "session_name", i ) );
    CheckReply( agent.CountSessions(), "{ \"count\" : 8 }" );
    CheckReply( agent.ListSessions( -1,  1 ), "[" + list.front()->ToJson() + "]" );
    CheckReply( agent.ListSessions(  8,  1 ), "[]" );
    CheckReply( agent.ListSessions(  0, -1 ), "[]" );
    CheckReply( agent.ListSessions(  7,  2 ), "[" + list.back()->ToJson() + "]" );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_session, Fixture )
{
    boost::shared_ptr< MockSession > session = AddSession( "exercise_name", "session_name", 10000 );
    MOCK_EXPECT( session->Stop ).once();
    CheckReply( agent.DeleteSession( session->GetTag() ), session->ToJson() );
    CheckReply( agent.CountSessions(), "{ \"count\" : 0 }" );
    CheckReply( agent.ListSessions( 0, 10 ), "[]" );
}