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
#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>
#include <web/Controller.h>
#include <web/Observer_ABC.h>
#include <web/Request_ABC.h>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace process;
using namespace web;

namespace
{
    MOCK_BASE_CLASS( MockRuntime, Runtime_ABC )
    {
        MOCK_METHOD( GetProcesses, 0 );
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

    MOCK_BASE_CLASS( MockRequest, Request_ABC )
    {
        MOCK_METHOD( GetMethod, 0 );
        MOCK_METHOD( GetUri, 0 );
        MOCK_METHOD( GetParameter, 1 );
        MOCK_METHOD( GetHeader, 1 );
    };

    Runtime_ABC::T_Processes FakeProcesses( int size)
    {
        Runtime_ABC::T_Processes list;
        for( int idx = 0; idx < size; ++idx )
            list.push_back( boost::make_shared< MockProcess >( idx + 1, "process_" +
                boost::lexical_cast< std::string >( idx + 1 ) ) );
        return list;
    }

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

    template< int size >
    struct RuntimeFixture
    {
        RuntimeFixture()
            : processes( FakeProcesses( size ) )
        {
            MOCK_EXPECT( runtime.GetProcesses ).once().returns( processes );
        }
        Runtime_ABC::T_Processes processes;
        MockRuntime runtime;
    };

    template< int size >
    struct Fixture : public RuntimeFixture< size >
    {
        Fixture()
            : controller( RuntimeFixture< size >::runtime )
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
        Controller controller;
    };
}

BOOST_FIXTURE_TEST_CASE( web_controller_lists, Fixture< 16 > )
{
    SetRequest( "GET", "/list", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit", "3" )
    );
    CheckNotify( 200, "[ "
        "{ \"pid\" : 6, \"name\" : \"process_6\" }, "
        "{ \"pid\" : 7, \"name\" : \"process_7\" }, "
        "{ \"pid\" : 8, \"name\" : \"process_8\" }"
        " ]"
    );
}

BOOST_FIXTURE_TEST_CASE( web_controller_clips_invalid_list_parameters, Fixture< 16 > )
{
    SetRequest( "GET", "/list", boost::assign::map_list_of
        ( "offset", "-20" )
        ( "limit", "2" )
    );
    CheckNotify( 200, "[ "
        "{ \"pid\" : 1, \"name\" : \"process_1\" }, "
        "{ \"pid\" : 2, \"name\" : \"process_2\" }"
        " ]"
    );

    SetRequest( "GET", "/list", boost::assign::map_list_of
        ( "offset", "27" )
        ( "limit", "10" )
    );
    CheckNotify( 200, "[  ]" );

    SetRequest( "GET", "/list", boost::assign::map_list_of
        ( "offset", "5" )
        ( "limit", "-20" )
    );
    CheckNotify( 200, "[  ]" );

    SetRequest( "GET", "/list", boost::assign::map_list_of
        ( "offset", "14" )
        ( "limit", "18" )
    );
    CheckNotify( 200, "[ "
        "{ \"pid\" : 15, \"name\" : \"process_15\" }, "
        "{ \"pid\" : 16, \"name\" : \"process_16\" }"
        " ]"
    );
}

BOOST_FIXTURE_TEST_CASE( web_controller_starts, Fixture< 5 > )
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
    MOCK_EXPECT( runtime.Start ).once().with( app, args, run ).returns( boost::make_shared< MockProcess >( 42, "Zebulon" ) );
    CheckNotify( 200, "{ \"pid\" : 42, \"name\" : \"Zebulon\" }" );
}

BOOST_FIXTURE_TEST_CASE( web_controller_stops, Fixture< 16 > )
{
    const int pid = 7;
    SetRequest( "GET", "/stop", boost::assign::map_list_of( "pid", boost::lexical_cast< std::string >( pid ) ) );
    MockProcess& process = dynamic_cast< MockProcess& >( *processes[ pid - 1 ] );
    MOCK_EXPECT( process.Kill ).once().with( mock::any ).returns( true );
    CheckNotify( 200, "" );
}
