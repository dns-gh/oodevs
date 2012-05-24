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
#include <host/PropertyTree.h>

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using mocks::MockFileSystem;
using mocks::MockPort;
using mocks::MockPortFactory;
using mocks::MockProcess;
using mocks::MockRuntime;
using mocks::MockUnpack;

namespace
{
    const std::string defaultIdString = "12345678-90ab-cdef-9876-543210123456";
    const Uuid defaultId = boost::uuids::string_generator()( defaultIdString );
    const std::string defaultName = "myName";
    const int defaultPort = 1337;
    const int processPid = 7331;
    const std::string processName = "myProcessName";
    const std::string defaultRoot = "root";

    struct Fixture
    {
        typedef boost::shared_ptr< Node > NodePtr;
        typedef boost::shared_ptr< MockProcess > ProcessPtr;
        MockFileSystem system;
        MockRuntime runtime;
        MockPortFactory ports;
        MOCK_FUNCTOR( Starter, ProcessPtr( const Node& ) );
        Fixture()
        {
            MOCK_EXPECT( system.IsFile ).returns( false );
        }

        NodePtr MakeNode()
        {
            return boost::make_shared< Node >( system, defaultRoot, defaultId, defaultName, std::auto_ptr< Port_ABC >( new MockPort( defaultPort ) ) );
        }

        NodePtr ReloadNode( const Tree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            return boost::make_shared< Node >( system, tree, runtime, ports );
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
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base + ","
        "\"root\":\"root\""
        "}" );
    ProcessPtr process = StartNode( *node, processPid, processName );
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base + ","
        "\"status\":\"running\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base + ","
        "\"root\":\"root\","
        "\"process\":{"
            "\"pid\":\"7331\","
            "\"name\":\"myProcessName\""
        "}}" );
    StopNode( *node, process );
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base + ","
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base + ","
        "\"root\":\"root\""
        "}" );
}

BOOST_FIXTURE_TEST_CASE( node_reloads, Fixture )
{
    const Tree save = MakeNode()->Save();
    ReloadNode( save );
}

BOOST_FIXTURE_TEST_CASE( node_starts_and_reloads, Fixture )
{
    NodePtr node = MakeNode();
    ProcessPtr process = StartNode( *node, processPid, processName );
    const Tree save = node->Save();
    node = ReloadNode( save, process );
    StopNode( *node, process );
}

BOOST_FIXTURE_TEST_CASE( node_rejects_bind_to_another_process, Fixture )
{
    NodePtr node = MakeNode();
    StartNode( *node, processPid, processName );
    const Tree save = node->Save();
    node = ReloadNode( save, boost::make_shared< MockProcess >( processPid, processName + "_" ) );
    BOOST_CHECK( node->Stop() );
}

BOOST_FIXTURE_TEST_CASE( node_can_start_twice, Fixture )
{
    NodePtr node = MakeNode();
    StartNode( *node, processPid, processName );
    BOOST_CHECK( node->Start( Starter ) );
}

BOOST_FIXTURE_TEST_CASE( node_pack, Fixture )
{
    NodePtr node = MakeNode();
    BOOST_CHECK_EQUAL( ToJson( node->GetPack() ), "{}" );

    mock::reset( system );
    const host::Path stash = host::Path( "root" ) / defaultIdString / "stash";
    MOCK_EXPECT( system.Remove ).once().with( stash );
    MOCK_EXPECT( system.MakeDirectory ).once().with( stash );

    boost::shared_ptr< MockUnpack > unpack = boost::make_shared< MockUnpack >();
    std::stringstream stream;
    MOCK_EXPECT( system.Unpack ).once().with( stash, boost::ref( stream ) ).returns( unpack );
    MOCK_EXPECT( unpack->Unpack ).once();

    MOCK_EXPECT( system.IsFile ).with( stash / "content.xml" ).returns( true );
    MOCK_EXPECT( system.ReadFile ).with( stash / "content.xml" ).returns(
        "<content>"
        "<name>my_name</name>"
        "<description>my_version</description>"
        "<version>some_version</version>"
        "</content>" );
    MOCK_EXPECT( system.Glob ).with( stash / "data/models", mock::any ).returns( false );
    MOCK_EXPECT( system.Glob ).with( stash / "data/terrains", mock::any ).returns( false );
    MOCK_EXPECT( system.Glob ).with( stash / "exercises", mock::any ).returns( false );

    const std::string expected = "{\"name\":\"my_name\",\"description\":\"my_version\",\"version\":\"some_version\",\"items\":\"\"}";
    node->ReadPack( stream );
    BOOST_CHECK_EQUAL( ToJson( node->GetPack() ), expected );
    BOOST_CHECK_EQUAL( ToJson( node->DeletePack() ), expected );
    BOOST_CHECK_EQUAL( ToJson( node->GetPack() ), "{}" );
}