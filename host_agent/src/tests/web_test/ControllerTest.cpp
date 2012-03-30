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

#include "web_test.h"

#include <host/Agent_ABC.h>

#include <web/Controller.h>
#include <web/Observer_ABC.h>
#include <web/Request_ABC.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace web;

namespace
{
    const std::string default_id_string = "0123456789abcdef0123456789abcdef";
    const boost::uuids::uuid default_id = boost::uuids::string_generator()( default_id_string );

    MOCK_BASE_CLASS( MockAgent, host::Agent_ABC )
    {
        MOCK_METHOD( ListSessions, 2 );
        MOCK_METHOD( CountSessions, 0 );
        MOCK_METHOD( GetSession, 1 );
        MOCK_METHOD( CreateSession, 2 );
        MOCK_METHOD( DeleteSession, 1 );
        MOCK_METHOD( StartSession, 1 );
        MOCK_METHOD( StopSession, 1 );
        MOCK_METHOD( ListExercises, 2 );
        MOCK_METHOD( CountExercises, 0 );
    };

    MOCK_BASE_CLASS( MockRequest, Request_ABC )
    {
        MOCK_METHOD( GetMethod, 0 );
        MOCK_METHOD( GetUri, 0 );
        MOCK_METHOD( GetParameter, 1 );
        MOCK_METHOD( GetHeader, 1 );
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

    struct Fixture
    {
        Fixture()
            : controller( agent )
        {
            // NOTHING
        }
        void SetRequest( const std::string& method, const std::string& uri, const std::map< std::string, std::string >& parameters = std::map< std::string, std::string >() )
        {
            MOCK_EXPECT( request.GetMethod ).once().returns( method );
            MOCK_EXPECT( request.GetUri ).once().returns( uri );
            typedef std::map< std::string, std::string > Pair;
            BOOST_FOREACH( const Pair::value_type& pair, parameters )
                MOCK_EXPECT( request.GetParameter ).once().with( pair.first ).returns( pair.second );
        }
        void CheckNotify( int code, const std::string& expected )
        {
            const std::string reply = controller.Notify( request );
            BOOST_CHECK( CheckHttpCode( code, reply ) );
            BOOST_CHECK_EQUAL( expected, EraseHttpHeader( reply ));
        }
        MockRequest request;
        MockAgent agent;
        Controller controller;
    };
}

BOOST_FIXTURE_TEST_CASE( controller_list_sessions, Fixture )
{
    SetRequest( "GET", "/list_sessions", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::string expected = "a json list";
    MOCK_EXPECT( agent.ListSessions ).once().with( 5, 3 ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_count_sessions, Fixture )
{
    SetRequest( "GET", "/count_sessions" );
    const std::string expected = "a json number";
    MOCK_EXPECT( agent.CountSessions ).once().returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_get_session, Fixture )
{
    SetRequest( "GET", "/get_session", boost::assign::map_list_of( "id", default_id_string ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.GetSession ).once().with( default_id ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_create_session, Fixture )
{
    const std::string exercise = "exercise_name";
    const std::string name = "session_name";
    SetRequest( "GET", "/create_session", boost::assign::map_list_of
        ( "exercise", exercise )
        ( "name", name )
    );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.CreateSession ).once().with( exercise, name ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_delete_session, Fixture )
{
    SetRequest( "GET", "/delete_session", boost::assign::map_list_of( "id", default_id_string ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.DeleteSession ).once().with( default_id ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_reject_invalid_ids, Fixture )
{
    SetRequest( "GET", "/delete_session", boost::assign::map_list_of( "id", "1" ) );
    const std::string expected = "Invalid \"id\" parameter";
    CheckNotify( 400, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_start_session, Fixture )
{
    SetRequest( "GET", "/start_session", boost::assign::map_list_of( "id", default_id_string ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.StartSession ).once().with( default_id ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_stop_session, Fixture )
{
    SetRequest( "GET", "/stop_session", boost::assign::map_list_of( "id", default_id_string ) );
    const std::string expected = "a json session";
    MOCK_EXPECT( agent.StopSession ).once().with( default_id ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_list_exercises, Fixture )
{
    SetRequest( "GET", "/list_exercises", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit",  "3" )
    );
    const std::string expected = "a json list";
    MOCK_EXPECT( agent.ListExercises ).once().with( 5, 3 ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( controller_count_exercises, Fixture )
{
    SetRequest( "GET", "/count_exercises" );
    const std::string expected = "a json number";
    MOCK_EXPECT( agent.CountExercises ).once().returns( expected );
    CheckNotify( 200, expected );
}
