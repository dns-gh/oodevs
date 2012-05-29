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
using mocks::MockLog;
using mocks::MockFileSystem;
using mocks::MockPackage;
using mocks::MockPackageFactory;
using mocks::MockPool;
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
        MockLog log;
        MockPool pool;
        MockPackageFactory packages;
        boost::shared_ptr< MockPackage > installed;
        boost::shared_ptr< MockPackage > cache;
        MOCK_FUNCTOR( Starter, ProcessPtr( const Node_ABC& ) );

        Fixture()
            : installed( boost::make_shared< MockPackage >() )
            , cache    ( boost::make_shared< MockPackage >() )
        {
            MOCK_EXPECT( system.IsFile ).returns( false );
        }

        NodePtr MakeNode()
        {
            MOCK_EXPECT( packages.Make ).once().with( mock::any, true ).returns( installed );
            return boost::make_shared< Node >( packages, system, pool, defaultRoot, defaultId, defaultName, std::auto_ptr< Port_ABC >( new MockPort( defaultPort ) ) );
        }

        NodePtr ReloadNode( const Tree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( packages.Make ).once().with( mock::any, true ).returns( installed );
            MOCK_EXPECT( packages.Make ).once().with( mock::any, false ).returns( cache );
            MOCK_EXPECT( installed->Parse ).once().returns( true );
            MOCK_EXPECT( cache->Parse ).once().returns( true );
            MOCK_EXPECT( cache->Identify ).once().with( mock::same( *installed ) );
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            return boost::make_shared< Node >( packages, system, pool, tree, runtime, ports );
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

BOOST_FIXTURE_TEST_CASE( node_cache, Fixture )
{
    NodePtr node = MakeNode();
    BOOST_CHECK_EQUAL( ToJson( node->GetCache() ), "{}" );

    mock::reset( system );
    const host::Path path = host::Path( "root" ) / defaultIdString / "cache";
    MOCK_EXPECT( system.Remove ).once().with( path );
    MOCK_EXPECT( system.MakeDirectory ).once().with( path );

    std::stringstream stream;
    boost::shared_ptr< MockUnpack > unpack = boost::make_shared< MockUnpack >();
    MOCK_EXPECT( system.Unpack ).once().with( path, boost::ref( stream ) ).returns( unpack );
    MOCK_EXPECT( unpack->Unpack ).once();

    MOCK_EXPECT( packages.Make ).once().with( mock::any, false ).returns( cache );
    MOCK_EXPECT( cache->Parse ).once().returns( true );
    MOCK_EXPECT( cache->Identify ).once().with( mock::same( *installed ) );
    Tree tree;
    tree.put( "some", "data" );
    const std::string expected = ToJson( tree );
    MOCK_EXPECT( cache->GetProperties ).returns( tree );
    node->UploadCache( stream );
    BOOST_CHECK_EQUAL( ToJson( node->GetCache() ), expected );
    MOCK_EXPECT( cache->RemoveAll ).once();
    BOOST_CHECK_EQUAL( ToJson( node->DeleteCache() ), expected );
    BOOST_CHECK_EQUAL( ToJson( node->GetCache() ), "{}" );
}