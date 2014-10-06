// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test_pch.h"

#include "TreeHelpers.h"
#include "host/Node.h"
#include "runtime/PropertyTree.h"
#include "web/Plugins.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "MockLog.h"
#include "MockFileSystem.h"
#include "MockNodeObserver.h"
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
        MockFileSystem fs;
        MockRuntime runtime;
        MockPortFactory ports;
        MockUuidFactory uuids;
        MockLog log;
        MockPool pool;
        MockNodeObserver observer;
        MockPackageFactory packages;
        boost::shared_ptr< MockPackage > installed;
        boost::shared_ptr< MockPackage > cache;

        Fixture()
            : installed( boost::make_shared< MockPackage >() )
            , cache    ( boost::make_shared< MockPackage >() )
        {
            MOCK_EXPECT( fs.IsFile ).returns( false );
        }

        NodePtr MakeNode()
        {
            MOCK_EXPECT( packages.Make ).once().with( mock::any, true ).returns( installed );
            MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
            MOCK_EXPECT( ports.Create0 ).once().returns( new MockPort( defaultPort ) );
            MOCK_EXPECT( fs.Walk ).once().with( "plugins", mock::any, mock::any );
            web::Plugins plugins( fs, "plugins" );
            host::NodeDependencies deps( packages, fs, uuids, observer, runtime, plugins, pool, ports );
            web::node::Config cfg;
            cfg.name = defaultName;
            return boost::make_shared< Node >( deps, defaultRoot, 5*60, cfg.name, cfg );
        }

        NodePtr ReloadNode( const Tree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( packages.Make ).once().with( mock::any, true ).returns( installed );
            MOCK_EXPECT( installed->Parse ).once().returns( true );
            MOCK_EXPECT( installed->Identify ).once().with( mock::same( *installed ) );
            MOCK_EXPECT( installed->CountExercises ).once().returns( 13 );
            MOCK_EXPECT( installed->GetSize ).once().returns( 20 );
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            MOCK_EXPECT( fs.Walk ).once().with( "plugins", mock::any, mock::any );
            web::Plugins plugins( fs, "plugins" );
            host::NodeDependencies deps( packages, fs, uuids, observer, runtime, plugins, pool, ports );
            return boost::make_shared< Node >( deps, defaultRoot, 5*60, tree );
        }

        ProcessPtr StartNode( Node& node, int pid, const std::string& name )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( runtime.Start ).once().returns( process );
            BOOST_REQUIRE( node.Start( "node.exe", "web", "node", 0, 0, false ) );
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

    T_Constraints base;
    base.push_back( boost::bind( &EqualValue, _1, "id", "12345678-90ab-cdef-9876-543210123456" ) );
    base.push_back( boost::bind( &EqualValue, _1, "ident", "myName" ) );
    base.push_back( boost::bind( &EqualValue, _1, "port", "1337" ) );
    base.push_back( boost::bind( &EqualValue, _1, "name", "myName" ) );
    base.push_back( boost::bind( &EqualValue, _1, "sessions.max_play", "0" ) );
    base.push_back( boost::bind( &EqualValue, _1, "sessions.max_parallel", "0" ) );
    base.push_back( boost::bind( &EqualValue, _1, "sessions.reset", "true" ) );

    T_Constraints props;
    props.push_back( boost::bind( &EqualValue, _1, "num_exercises", "0" ) );
    props.push_back( boost::bind( &EqualValue, _1, "sessions.num_play", "0" ) );
    props.push_back( boost::bind( &EqualValue, _1, "sessions.num_parallel", "0" ) );
    props.push_back( boost::bind( &EqualValue, _1, "data_size", "0" ) );

    Tree src = node->GetProperties();
    Check( base, src );
    Check( props, src );
    EqualValue( src, "status", "stopped" );
    src = node->Save();
    Check( base, src );
    EqualValue( src, "sessions.num_play", "0" );
    EqualValue( src, "stopped", "false" );

    ProcessPtr process = StartNode( *node, processPid, processName );
    src = node->GetProperties();
    Check( base, src );
    Check( props, src );
    EqualValue( src, "status", "running" );
    src = node->Save();
    Check( base, src );
    EqualValue( src, "sessions.num_play", "0" );
    EqualValue( src, "stopped", "false" );
    EqualValue( src, "process.pid", "7331" );
    EqualValue( src, "process.name", "myProcessName" );

    StopNode( *node, process );
    src = node->GetProperties();
    Check( base, src );
    Check( props, src );
    EqualValue( src, "status", "stopped" );
    src = node->Save();
    Check( base, src );
    EqualValue( src, "sessions.num_play", "0" );
    EqualValue( src, "stopped", "true" );
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
    BOOST_CHECK( !node->Start( "node.exe", "web", "node", 0, 0, false ) );
}

BOOST_FIXTURE_TEST_CASE( node_cache, Fixture )
{
    NodePtr node = MakeNode();
    BOOST_CHECK( node->GetCache().empty() );

    mock::reset( fs );
    MOCK_EXPECT( fs.MakeAnyPath ).returns( "" );

    NilReader src;
    boost::shared_ptr< MockUnpack > unpack = boost::make_shared< MockUnpack >();
    MOCK_EXPECT( fs.Unpack ).once().with( mock::any, mock::same( src ), mock::any ).returns( unpack );
    MOCK_EXPECT( unpack->Unpack ).once();

    MOCK_EXPECT( packages.Make ).once().with( mock::any, false ).returns( cache );
    MOCK_EXPECT( cache->Parse ).once().returns( true );
    MOCK_EXPECT( cache->Identify ).once().with( mock::same( *installed ) );
    Tree tree;
    tree.put( "some", "data" );
    MOCK_EXPECT( cache->GetProperties ).returns( tree );
    MOCK_EXPECT( cache->GetSize ).returns( 0 );
    MOCK_EXPECT( uuids.Create ).returns( boost::uuids::random_generator()() );
    MOCK_EXPECT( fs.MakePaths );
    MOCK_EXPECT( fs.MakePath ).returns( true );
    MOCK_EXPECT( fs.Exists ).returns( false );
    MOCK_EXPECT( fs.IsDirectory ).returns( true );
    MOCK_EXPECT( fs.Rename ).returns( true );
    MOCK_EXPECT( fs.Remove ).returns( true );
    node->UploadCache( src );
    BOOST_CHECK( node->GetCache() ==  tree );
    MOCK_EXPECT( cache->GetPath ).once().returns( "" );
    BOOST_CHECK( node->DeleteCache() == tree );
    BOOST_CHECK( node->GetCache().empty() );
}
