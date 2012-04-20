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
#include <host/Node.h>
#include <host/Json.h>

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using mocks::MockPort;
using mocks::MockPortFactory;
using mocks::MockProcess;
using mocks::MockRuntime;

namespace
{
    const std::string defaultIdString = "12345678-90ab-cdef-9876-543210123456";
    const boost::uuids::uuid defaultId = boost::uuids::string_generator()( defaultIdString );
    const std::string defaultName = "myName";
    const int defaultPort = 1337;
    const int processPid = 7331;
    const std::string processName = "myProcessName";

    struct Fixture
    {
        typedef boost::shared_ptr< Node > NodePtr;
        typedef boost::shared_ptr< MockProcess > ProcessPtr;
        MockRuntime runtime;
        MockPortFactory ports;
        MOCK_FUNCTOR( Starter, ProcessPtr( const Node& ) );
        Fixture()
        {
            // NOTHING
        }

        NodePtr MakeNode()
        {
            return boost::make_shared< Node >( defaultId, defaultName, std::auto_ptr< Port_ABC >( new MockPort( defaultPort ) ) );
        }

        NodePtr ReloadNode( const boost::property_tree::ptree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            return boost::make_shared< Node >( tree, runtime, ports );
        }

        ProcessPtr StartNode( Node& node, int pid, const std::string& name )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( Starter ).with( mock::same( node ) ).returns( process );
            BOOST_REQUIRE( node.Start( Starter ) );
            return process;
        }

        void StopNode( Node& node, ProcessPtr process = ProcessPtr() )
        {
            if( process )
                MOCK_EXPECT( process->Kill ).once().returns( true );
            BOOST_CHECK( node.Stop() );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( node_constructs, Fixture )
{
    NodePtr node = MakeNode();
    BOOST_CHECK_EQUAL( node->GetId(), defaultId );
}

BOOST_FIXTURE_TEST_CASE( node_starts_and_stops, Fixture )
{
    NodePtr node = MakeNode();
    ProcessPtr process = StartNode( *node, processPid, processName );
    StopNode( *node, process );
}

BOOST_FIXTURE_TEST_CASE( node_converts, Fixture )
{
    NodePtr node = MakeNode();
    const std::string base = "{"
        "\"id\":\"12345678-90ab-cdef-9876-543210123456\","
        "\"name\":\"myName\","
        "\"port\":\"1337\"";
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base + ","
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base +
        "}" );
    ProcessPtr process = StartNode( *node, processPid, processName );
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base + ","
        "\"status\":\"running\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base + ","
        "\"process\":{"
            "\"pid\":\"7331\","
            "\"name\":\"myProcessName\""
        "}}" );
    StopNode( *node, process );
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base + ","
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base +
        "}" );
}

BOOST_FIXTURE_TEST_CASE( node_reloads, Fixture )
{
    const boost::property_tree::ptree save = MakeNode()->Save();
    ReloadNode( save );
}

BOOST_FIXTURE_TEST_CASE( node_starts_and_reloads, Fixture )
{
    NodePtr node = MakeNode();
    ProcessPtr process = StartNode( *node, processPid, processName );
    const boost::property_tree::ptree save = node->Save();
    node = ReloadNode( save, process );
    StopNode( *node, process );
}

BOOST_FIXTURE_TEST_CASE( node_rejects_bind_to_another_process, Fixture )
{
    NodePtr node = MakeNode();
    StartNode( *node, processPid, processName );
    const boost::property_tree::ptree save = node->Save();
    node = ReloadNode( save, boost::make_shared< MockProcess >( processPid, processName + "_" ) );
    BOOST_CHECK( node->Stop() );
}

BOOST_FIXTURE_TEST_CASE( node_can_start_twice, Fixture )
{
    NodePtr node = MakeNode();
    StartNode( *node, processPid, processName );
    BOOST_CHECK( node->Start( Starter ) );
}
