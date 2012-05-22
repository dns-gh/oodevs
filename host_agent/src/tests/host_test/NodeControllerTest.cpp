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
#include <host/PropertyTree.h>
#include <host/NodeController.h>

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/uuid/string_generator.hpp>

using namespace host;
using mocks::MockLog;
using mocks::MockRuntime;
using mocks::MockFileSystem;
using mocks::MockUuidFactory;
using mocks::MockProxy;
using mocks::MockPool;
using mocks::MockPort;
using mocks::MockPortFactory;
using mocks::MockProcess;
using mocks::MockUnpack;

namespace
{
    struct SubFixture
    {
        SubFixture( const std::string& root, const std::string& java, const std::string& jar, const std::string& web )
        {
            MOCK_EXPECT( system.MakeDirectory ).with( root + "/node" );
            MOCK_EXPECT( system.Exists ).with( java ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( java ).returns( true );
            MOCK_EXPECT( system.Exists ).with( jar ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( jar ).returns( true );
            MOCK_EXPECT( system.IsFile ).returns( false );
            MOCK_EXPECT( system.IsDirectory ).with( web ).returns( true );
            MOCK_EXPECT( proxy.GetPort ).returns( 8080 );
        };
        MockLog log;
        MockRuntime runtime;
        MockFileSystem system;
        MockUuidFactory uuids;
        MockProxy proxy;
        MockPool pool;
        MockPortFactory ports;
    };

    const std::string idActiveText = "12345678-1234-1234-1234-123456789abc";
    const boost::uuids::uuid idActive = boost::uuids::string_generator()( idActiveText );
    const std::string nodeActive = "{"
                                   "\"id\":\"" + idActiveText + "\","
                                   "\"name\":\"myName\","
                                   "\"port\":\"1337\","
                                   "\"process\":{\"pid\":\"1234\",\"name\":\"e:/java/some_java.exe\"}"
                                   "}";

    const std::string idIdleText = "87654321-4321-4321-4321-cba987654321";
    const boost::uuids::uuid idIdle = boost::uuids::string_generator()( idIdleText );
    const std::string nodeIdle = "{"
                                 "\"id\":\"" + idIdleText + "\","
                                 "\"name\":\"myName2\","
                                 "\"port\":\"1338\""
                                 "}";

    template< bool isCluster = false >
    struct Fixture
    {
        Fixture()
            : root   ( "e:/root" )
            , java   ( "e:/java/some_java.exe" )
            , jar    ( "e:/jar/some_jar.jar" )
            , web    ( "e:/zomg/www" )
            , type   ( isCluster ? "cluster" : "node" )
            , sub    ( root, java, jar, web )
            , control( sub.log, sub.runtime, sub.system, sub.uuids, sub.proxy, root, java, jar, web, type, sub.pool, sub.ports )
        {
            // NOTHING
        }
        const std::string root;
        const std::string java;
        const std::string jar;
        const std::string web;
        const std::string type;
        SubFixture sub;
        NodeController control;

        boost::shared_ptr< MockProcess > Reload()
        {
            MOCK_EXPECT( sub.system.Glob ).once().with( root + "/node", L"node.id" ).returns( boost::assign::list_of< boost::filesystem::path >( "a/b/c/node.id" )( "node.id" ) );

            MOCK_EXPECT( sub.system.ReadFile ).once().with( "a/b/c/node.id" ).returns( nodeActive );
            MOCK_EXPECT( sub.ports.Create1 ).once().with( 1337 ).returns( new MockPort( 1337 ) );
            boost::shared_ptr< MockProcess > process = boost::make_shared< MockProcess >( 1234, "e:/java/some_java.exe" );
            MOCK_EXPECT( sub.runtime.GetProcess ).once().with( 1234 ).returns( process );
            MOCK_EXPECT( sub.proxy.Register ).once().with( idActiveText, "localhost", 1337 );
            MOCK_EXPECT( sub.system.WriteFile ).once().with( root + "/node/" + idActiveText + "/node.id", mock::any );

            MOCK_EXPECT( sub.system.ReadFile ).once().with( "node.id" ).returns( nodeIdle );
            MOCK_EXPECT( sub.ports.Create1 ).once().with( 1338 ).returns( new MockPort( 1338 ) );
            MOCK_EXPECT( sub.proxy.Register ).once().with( idIdleText, "localhost", 1338 );
            MOCK_EXPECT( sub.system.WriteFile ).once().with( root + "/node/" + idIdleText + "/node.id", mock::any );
            control.Reload();
            return process;
        }
    };
}

BOOST_FIXTURE_TEST_CASE( node_controller_reloads, Fixture<> )
{
    Reload();
    BOOST_CHECK_EQUAL( control.Count(), size_t( 2 ) );
    NodeController::T_Nodes nodes = control.List( 0, INT_MAX );
    BOOST_CHECK_EQUAL( nodes.size(), size_t( 2 ) );
    BOOST_CHECK( control.Get( idActive ) );
    BOOST_CHECK( control.Get( idIdle ) );
}

BOOST_FIXTURE_TEST_CASE( node_controller_creates, Fixture<> )
{
    MOCK_EXPECT( sub.ports.Create0 ).once().returns( new MockPort( 1337 ) );
    MOCK_EXPECT( sub.uuids.Create ).once().returns( idIdle );
    MOCK_EXPECT( sub.proxy.Register ).once().with( idIdleText, "localhost", 1337 );
    MOCK_EXPECT( sub.runtime.Start ).once().with( java, boost::assign::list_of< std::string >
        ( " -jar \"some_jar.jar\"" )
        ( "--root \"e:/zomg/www\"" )
        ( "--proxy \"8080\"" )
        ( "--uuid \"" + idIdleText + "\"" )
        ( "--type \"node\"" )
        ( "--name \"zebulon\"" )
        ( "--port \"1337\"" ),
        "e:/jar", mock::any ).returns( boost::make_shared< MockProcess >( 1377, java ) );
    MOCK_EXPECT( sub.system.WriteFile ).once();
    MOCK_EXPECT( sub.system.MakeDirectory ).once().with( root + "/node/" + idIdleText );
    NodeController::T_Node node = control.Create( "zebulon" );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );
    BOOST_CHECK( control.Has( idIdle ) );
    BOOST_CHECK_EQUAL( control.Get( idIdle ), node );
    NodeController::T_Nodes nodes = control.List( 0, INT_MAX );
    BOOST_REQUIRE_EQUAL( nodes.size(), size_t( 1 ) );
    BOOST_CHECK_EQUAL( nodes.front(), node );
}

BOOST_FIXTURE_TEST_CASE( node_controller_deletes, Fixture<> )
{
    boost::shared_ptr< MockProcess > process = Reload();
    MOCK_EXPECT( sub.proxy.Unregister ).once().with( idActiveText );
    MOCK_EXPECT( process->Kill ).once().returns( true );
    MOCK_EXPECT( sub.system.Remove );
    NodeController::T_Node node = control.Delete( idActive );
    BOOST_CHECK_EQUAL( node->GetId(), idActive );
    BOOST_CHECK( !control.Has( idActive ) );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );
    MOCK_EXPECT( sub.proxy.Unregister ).once().with( idIdleText );
    node = control.Delete( idIdle );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 0 ) );
}

BOOST_FIXTURE_TEST_CASE( node_controller_starts, Fixture<> )
{
    Reload();
    MOCK_EXPECT( sub.system.WriteFile ).once();
    MOCK_EXPECT( sub.runtime.Start ).once().returns( boost::make_shared< MockProcess >( 1398, java ) );
    NodeController::T_Node node = control.Start( idIdle );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
}

BOOST_FIXTURE_TEST_CASE( node_controller_stops, Fixture<> )
{
    boost::shared_ptr< MockProcess > ptr = Reload();
    MOCK_EXPECT( sub.system.WriteFile ).once();
    MOCK_EXPECT( ptr->Kill ).once().returns( true );
    NodeController::T_Node node = control.Stop( idActive );
    BOOST_CHECK_EQUAL( node->GetId(), idActive );
}

BOOST_FIXTURE_TEST_CASE( node_controller_upload_pack, Fixture<> )
{
    Reload();
    std::istringstream stream;
    MOCK_EXPECT( sub.system.Remove );
    MOCK_EXPECT( sub.system.MakeDirectory );
    boost::shared_ptr< MockUnpack > unpack = boost::make_shared< MockUnpack >();
    MOCK_EXPECT( sub.system.Unpack ).once().with( mock::any, boost::ref( stream ) ).returns( unpack );
    MOCK_EXPECT( unpack->Unpack ).once();
    NodeController::T_Tree tree = control.UploadPack( idIdle, stream );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_get_pack, Fixture<> )
{
    Reload();
    NodeController::T_Tree tree = control.GetPack( idIdle );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_delete_pack, Fixture<> )
{
    Reload();
    NodeController::T_Tree tree = control.DeletePack( idIdle );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}