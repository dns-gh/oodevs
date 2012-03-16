// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>

#include <host/Agent.h>

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

using namespace runtime;
using namespace host;

namespace
{
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

    Runtime_ABC::T_Processes FakeProcesses( int size)
    {
        Runtime_ABC::T_Processes list;
        for( int idx = 0; idx < size; ++idx )
            list.push_back( boost::make_shared< MockProcess >( idx + 1, "process_" +
                boost::lexical_cast< std::string >( idx + 1 ) ) );
        return list;
    }

    template< int size >
    struct RuntimeFixture
    {
        RuntimeFixture()
            : processes( FakeProcesses( size ) )
        {
            MOCK_EXPECT( runtime.GetProcesses ).returns( processes );
        }
        Runtime_ABC::T_Processes processes;
        MockRuntime runtime;
    };

    template< int size >
    struct Fixture : public RuntimeFixture< size >
    {
        Fixture()
            : agent( RuntimeFixture< size >::runtime )
        {
            // NOTHING
        }
        void CheckReply( const Reply& reply, const std::string& expected, bool valid = true )
        {
            BOOST_CHECK_EQUAL( reply.valid, valid );
            BOOST_CHECK_EQUAL( reply.data, expected );
        }
        Agent agent;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_lists, Fixture< 16 > )
{
    CheckReply( agent.List( 5, 3 ), "["
        "{ \"pid\" : 6, \"name\" : \"process_6\" }, "
        "{ \"pid\" : 7, \"name\" : \"process_7\" }, "
        "{ \"pid\" : 8, \"name\" : \"process_8\" }"
        "]"
    );
}

BOOST_FIXTURE_TEST_CASE( agent_clips_invalid_list_parameters, Fixture< 16 > )
{
    CheckReply( agent.List( -20, 2 ), "["
        "{ \"pid\" : 1, \"name\" : \"process_1\" }, "
        "{ \"pid\" : 2, \"name\" : \"process_2\" }"
        "]"
    );

    CheckReply( agent.List( 27, 20 ), "[]" );

    CheckReply( agent.List( 5, -20 ), "[]" );

    CheckReply( agent.List( 14, 18 ), "["
        "{ \"pid\" : 15, \"name\" : \"process_15\" }, "
        "{ \"pid\" : 16, \"name\" : \"process_16\" }"
        "]"
    );
}

BOOST_FIXTURE_TEST_CASE( agent_starts, Fixture< 5 > )
{
    const std::string app = "e:/my_app.exe", run = "e:/run_dir/vc100";
    const std::vector< std::string> args = boost::assign::list_of
        ( "--root-dir=../../data" )
        ( "--exercise=worldwide/Egypt" )
        ( "--session=default" );
    MOCK_EXPECT( runtime.Start ).once().with( app, args, run ).returns( boost::make_shared< MockProcess >( 42, "Zebulon" ) );
    CheckReply( agent.Start( app, args, run ), "{ \"pid\" : 42, \"name\" : \"Zebulon\" }" );
}

BOOST_FIXTURE_TEST_CASE( agent_stops, Fixture< 16 > )
{
    const int pid = 7;
    boost::shared_ptr< Process_ABC > ptr = processes[ pid - 1 ];
    MockProcess& process = dynamic_cast< MockProcess& >( *ptr );
    MOCK_EXPECT( runtime.GetProcess ).once().with( pid ).returns( ptr );
    MOCK_EXPECT( process.Kill ).once().with( mock::any ).returns( true );
    CheckReply( agent.Stop( pid ), "{ \"pid\" : 7, \"name\" : \"process_7\" }" );
}
