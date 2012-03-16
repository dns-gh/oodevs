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
#include <boost/xpressive/xpressive.hpp>

using namespace web;

namespace
{
    MOCK_BASE_CLASS( MockAgent, host::Agent_ABC )
    {
        MOCK_METHOD( List, 2 );
        MOCK_METHOD( Start, 3 );
        MOCK_METHOD( Stop, 1 );
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
        void SetRequest( const std::string& method, const std::string& uri, const std::map< std::string, std::string >& parameters )
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

BOOST_FIXTURE_TEST_CASE( web_controller_lists, Fixture )
{
    SetRequest( "GET", "/list", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit", "3" )
    );
    const std::string expected = "some data";
    MOCK_EXPECT( agent.List ).once().with( 5, 3 ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( web_controller_starts, Fixture )
{
    const std::string app = "e:/my_app.exe", run = "e:/run_dir/vc100";
    const std::vector< std::string> args = boost::assign::list_of
        ( "--root-dir=../../data" )
        ( "--exercise=worldwide/Egypt" )
        ( "--session=default" );
    SetRequest( "GET" , "/start", boost::assign::map_list_of
        ( "app", app )
        ( "cmd", boost::algorithm::join( args, "," ) )
        ( "run", run )
    );
    const std::string expected = "some data";
    MOCK_EXPECT( agent.Start ).once().with( app, args, run ).returns( expected );
    CheckNotify( 200, expected );
}

BOOST_FIXTURE_TEST_CASE( web_controller_stops, Fixture )
{
    const int pid = 7;
    SetRequest( "GET", "/stop", boost::assign::map_list_of( "pid", boost::lexical_cast< std::string >( pid ) ) );
    const std::string expected = "some data";
    MOCK_EXPECT( agent.Stop ).once().with( pid ).returns( expected );
    CheckNotify( 200, expected );
}
