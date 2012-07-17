// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/Node.h"
#include "runtime/PropertyTree.h"

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "MockLog.h"
#include "MockFileSystem.h"
#include "MockPackage.h"
#include "MockPool.h"
#include "MockPortFactory.h"
#include "MockProcess.h"
#include "MockRuntime.h"
#include "MockUuidFactory.h"

using namespace host;
using namespace mocks;
using namespace property_tree;

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
        MockUuidFactory uuids;
        MockLog log;
        MockPool pool;
        MockPackageFactory packages;
        boost::shared_ptr< MockPackage > installed;
        boost::shared_ptr< MockPackage > cache;

        Fixture()
            : installed( boost::make_shared< MockPackage >() )
            , cache    ( boost::make_shared< MockPackage >() )
        {
            MOCK_EXPECT( system.IsFile ).returns( false );
        }

        NodePtr MakeNode()
        {
            MOCK_EXPECT( packages.Make ).once().with( mock::any, true ).returns( installed );
            MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
            MOCK_EXPECT( ports.Create0 ).once().returns( new MockPort( defaultPort ) );
            return boost::make_shared< Node >( packages, system, uuids, pool, defaultRoot, defaultName, 16, 8, ports );
        }

        NodePtr ReloadNode( const Tree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( packages.Make ).once().with( mock::any, true ).returns( installed );
            MOCK_EXPECT( installed->Parse ).once().returns( true );
            MOCK_EXPECT( installed->Identify ).once().with( mock::same( *installed ) );
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            return boost::make_shared< Node >( packages, system, uuids, pool, "", tree, runtime, ports );
        }

        ProcessPtr StartNode( Node& node, int pid, const std::string& name )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( runtime.Start ).once().returns( process );
            BOOST_REQUIRE( node.Start( runtime, "java", "jar", "web", "node", 0, false ) );
            return process;
        }

        void StopNode( Node& node, ProcessPtr process = ProcessPtr() )
        {
            if( process )
                MOCK_EXPECT( process->Kill ).once().returns( true );
            BOOST_CHECK( node.Stop( false ) );
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
        "\"port\":\"1337\","
        "\"max_sessions\":\"16\","
        "\"parallel_sessions\":\"8\",";
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base +
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base +
        "\"stopped\":\"false\""
        "}" );
    ProcessPtr process = StartNode( *node, processPid, processName );
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base +
        "\"status\":\"running\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base +
        "\"stopped\":\"false\","
        "\"process\":{"
            "\"pid\":\"7331\","
            "\"name\":\"myProcessName\""
        "}}" );
    StopNode( *node, process );
    BOOST_CHECK_EQUAL( ToJson( node->GetProperties() ), base +
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( node->Save() ), base +
        "\"stopped\":\"true\""
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
    BOOST_CHECK( node->Stop( false ) );
}

BOOST_FIXTURE_TEST_CASE( node_can_start_twice, Fixture )
{
    NodePtr node = MakeNode();
    StartNode( *node, processPid, processName );
    BOOST_CHECK( !node->Start( runtime, "java", "jar", "web", "node", 0, false ) );
}

BOOST_FIXTURE_TEST_CASE( node_cache, Fixture )
{
    NodePtr node = MakeNode();
    BOOST_CHECK_EQUAL( ToJson( node->GetCache() ), "{}" );

    mock::reset( system );
    MOCK_EXPECT( system.MakeAnyPath ).returns( "" );

    std::stringstream stream;
    boost::shared_ptr< MockUnpack > unpack = boost::make_shared< MockUnpack >();
    MOCK_EXPECT( system.Unpack ).once().with( mock::any, boost::ref( stream ) ).returns( unpack );
    MOCK_EXPECT( unpack->Unpack ).once();

    MOCK_EXPECT( packages.Make ).once().with( mock::any, false ).returns( cache );
    MOCK_EXPECT( cache->Parse ).once().returns( true );
    MOCK_EXPECT( cache->Identify ).once().with( mock::same( *installed ) );
    Tree tree;
    tree.put( "some", "data" );
    const std::string expected = ToJson( tree );
    MOCK_EXPECT( cache->GetProperties ).returns( tree );
    MOCK_EXPECT( uuids.Create ).returns( boost::uuids::random_generator()() );
    MOCK_EXPECT( system.MakePaths );
    MOCK_EXPECT( system.MakePath ).returns( true );
    MOCK_EXPECT( system.Exists ).returns( false );
    MOCK_EXPECT( system.IsDirectory ).returns( true );
    MOCK_EXPECT( system.Rename ).returns( true );
    MOCK_EXPECT( system.Remove ).returns( true );
    node->UploadCache( stream );
    BOOST_CHECK_EQUAL( ToJson( node->GetCache() ), expected );
    MOCK_EXPECT( cache->GetPath ).once().returns( "" );
    BOOST_CHECK_EQUAL( ToJson( node->DeleteCache() ), expected );
    BOOST_CHECK_EQUAL( ToJson( node->GetCache() ), "{}" );
}