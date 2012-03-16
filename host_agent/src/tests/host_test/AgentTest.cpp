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

#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace runtime;
using namespace host;

namespace
{
    const std::string default_tag_string = "01234567-89ab-cdef-0123-456789abcdef";
    const boost::uuids::uuid default_tag = boost::uuids::string_generator()( default_tag_string );
    const boost::xpressive::sregex session_regex = boost::xpressive::sregex::compile(
        "{ \"tag\" : \"[0-9a-fA-F]{8,8}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{12,12}\", \"pid\" : -?\\d+, \"name\" : \"\" }" );

    MOCK_BASE_CLASS( MockRuntime, Runtime_ABC )
    {
        MOCK_METHOD( GetProcesses, 0 );
        MOCK_METHOD( GetProcess, 1 );
        MOCK_METHOD( Start, 3 );
    };

    MOCK_BASE_CLASS( MockProcess, Process_ABC )
    {
        MockProcess( int pid, const std::string& name )
        {
            MOCK_EXPECT( GetPid ).returns( pid );
            MOCK_EXPECT( GetName ).returns( name );
        }
        MOCK_METHOD( GetPid, 0 );
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( Join, 1 );
        MOCK_METHOD( Kill, 1 );
    };

    struct Fixture
    {
        Fixture()
            : agent( runtime )
        {
            // NOTHING
        }
        void CheckReply( const Reply& reply, const std::string& expected, bool valid = true )
        {
            BOOST_CHECK_EQUAL( reply.valid, valid );
            BOOST_CHECK_EQUAL( reply.data, expected );
        }
        void RegexReply( const Reply& reply, const boost::xpressive::sregex& expected, bool valid = true )
        {
            BOOST_CHECK_EQUAL( reply.valid, valid );
            BOOST_CHECK( boost::xpressive::regex_match( reply.data, expected, boost::xpressive::regex_constants::format_perl ) );
        }
        MockRuntime runtime;
        Agent agent;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_list_sessions, Fixture )
{
    CheckReply( agent.ListSessions( 5, 3 ), "[]" );
}

BOOST_FIXTURE_TEST_CASE( agent_count_sessions, Fixture )
{
    CheckReply( agent.CountSessions(), "{ \"count\" : 0 }" );
}

BOOST_FIXTURE_TEST_CASE( agent_get_session, Fixture )
{
    CheckReply( agent.GetSession( default_tag ), "unable to find session " + default_tag_string, false );
}

BOOST_AUTO_TEST_CASE( session_regex_works )
{
    boost::format fmt( "{ \"tag\" : \"%1%\", \"pid\" : %2%, \"name\" : \"\" }" );
    BOOST_CHECK( boost::xpressive::regex_match( ( fmt % default_tag_string % -1 ).str(), session_regex ) );
    BOOST_CHECK( !boost::xpressive::regex_match( ( fmt % ( default_tag_string + "a" ) % -1 ).str(), session_regex ) );
    BOOST_CHECK( !boost::xpressive::regex_match( ( fmt % default_tag_string % "b" ).str(), session_regex ) );
}

BOOST_FIXTURE_TEST_CASE( agent_create_session, Fixture )
{
    RegexReply( agent.CreateSession( 8080 ), session_regex );
}

BOOST_FIXTURE_TEST_CASE( agent_delete_session, Fixture )
{
    CheckReply( agent.DeleteSession( default_tag ), "unable to find session " + default_tag_string, false );
}
