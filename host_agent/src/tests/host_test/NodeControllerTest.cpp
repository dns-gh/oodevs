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
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

using namespace host;
using mocks::MockFileSystem;
using mocks::MockLog;
using mocks::MockNode;
using mocks::MockNodeFactory;
using mocks::MockPool;
using mocks::MockProxy;
using mocks::MockRuntime;

namespace
{
    struct SubFixture
    {
        SubFixture( const Path& root, const Path& java, const Path& jar, const Path& web )
        {
            MOCK_EXPECT( system.MakeDirectory ).with( root / "node" );
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
        MockProxy proxy;
        MockPool pool;
        MockNodeFactory nodes;
    };

    const std::string idActiveText = "12345678-1234-1234-1234-123456789abc";
    const Uuid idActive = boost::uuids::string_generator()( idActiveText );
    const std::string nodeActive = "{"
                                   "\"id\":\"" + idActiveText + "\","
                                   "\"name\":\"myName\","
                                   "\"port\":\"1337\","
                                   "\"root\":\"root\","
                                   "\"process\":{\"pid\":\"1234\",\"name\":\"e:\\/java\\/some_java.exe\"}"
                                   "}";

    const std::string idIdleText = "87654321-4321-4321-4321-cba987654321";
    const Uuid idIdle = boost::uuids::string_generator()( idIdleText );
    const std::string nodeIdle = "{"
                                 "\"id\":\"" + idIdleText + "\","
                                 "\"name\":\"myName2\","
                                 "\"port\":\"1338\","
                                 "\"root\":\"root\""
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
            , control( sub.log, sub.runtime, sub.system, sub.proxy, sub.nodes, root, java, jar, web, type, sub.pool )
        {
            // NOTHING
        }
        const Path root;
        const Path java;
        const Path jar;
        const Path web;
        const std::string type;
        SubFixture sub;
        NodeController control;
        boost::shared_ptr< MockNode > active;
        boost::shared_ptr< MockNode > idle;

        boost::shared_ptr< MockNode > AddNode( const Uuid& id, const std::string& data, const Path& path = Path() )
        {
            const Tree tree = FromJson( data );
            boost::shared_ptr< MockNode > node = boost::make_shared< MockNode >( id, tree );
            const std::string idText = tree.get< std::string >( "id" );
            if( path.empty() )
            {
                MOCK_EXPECT( sub.system.MakeDirectory ).once().with( root / "node" / idText );
                MOCK_EXPECT( sub.nodes.Make2 ).once().with( mock::any, tree.get< std::string >( "name" ) ).returns( node );
                MOCK_EXPECT( node->Start ).once().returns( true );
            }
            else
            {
                MOCK_EXPECT( sub.system.ReadFile ).once().with( path ).returns( data );
                MOCK_EXPECT( sub.nodes.Make1 ).once().returns( node );
            }
            MOCK_EXPECT( sub.system.WriteFile ).once().with( root / "node" / idText / "node.id", data );
            MOCK_EXPECT( sub.proxy.Register ).once().with( idText, "localhost", node->GetPort() );
            return node;
        }

        void Reload()
        {
            MOCK_EXPECT( sub.system.Glob ).once().with( root / "node", L"node.id" ).returns( boost::assign::list_of< Path >( "a/b/c/node.id" )( "node.id" ) );

            active = AddNode( idActive, nodeActive, "a/b/c/node.id" );
            idle = AddNode( idIdle, nodeIdle, "node.id" );
            control.Reload();
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
    AddNode( idIdle, nodeIdle );
    NodeController::T_Node node = control.Create( "myName2" );
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
    Reload();

    MOCK_EXPECT( sub.system.Remove ).once();
    MOCK_EXPECT( sub.proxy.Unregister ).once().with( idActiveText );
    MOCK_EXPECT( active->Stop ).once().returns( true );
    NodeController::T_Node node = control.Delete( idActive );
    BOOST_CHECK_EQUAL( node->GetId(), idActive );
    BOOST_CHECK( !control.Has( idActive ) );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );

    MOCK_EXPECT( sub.system.Remove ).once();
    MOCK_EXPECT( sub.proxy.Unregister ).once().with( idIdleText );
    MOCK_EXPECT( idle->Stop ).once().returns( true );
    node = control.Delete( idIdle );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 0 ) );
}

BOOST_FIXTURE_TEST_CASE( node_controller_starts, Fixture<> )
{
    Reload();
    MOCK_EXPECT( idle->Start ).once().returns( true );
    MOCK_EXPECT( sub.system.WriteFile ).once();
    NodeController::T_Node node = control.Start( idIdle );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
}

BOOST_FIXTURE_TEST_CASE( node_controller_stops, Fixture<> )
{
    Reload();
    MOCK_EXPECT( active->Stop ).once().returns( true );
    MOCK_EXPECT( sub.system.WriteFile ).once();
    NodeController::T_Node node = control.Stop( idActive );
    BOOST_CHECK_EQUAL( node->GetId(), idActive );
}

BOOST_FIXTURE_TEST_CASE( node_controller_upload_pack, Fixture<> )
{
    Reload();
    std::istringstream stream;
    MOCK_EXPECT( idle->ReadPack ).once().with( boost::ref( stream ) );
    MOCK_EXPECT( idle->GetPack ).once().returns( Tree() );
    Tree tree = control.UploadPack( idIdle, stream );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_get_pack, Fixture<> )
{
    Reload();
    MOCK_EXPECT( idle->GetPack ).once().returns( Tree() );
    Tree tree = control.GetPack( idIdle );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_delete_pack, Fixture<> )
{
    Reload();
    MOCK_EXPECT( idle->DeletePack ).once().returns( Tree() );
    Tree tree = control.DeletePack( idIdle );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}
