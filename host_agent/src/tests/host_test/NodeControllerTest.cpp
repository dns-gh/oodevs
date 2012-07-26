// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/NodeController.h"
#include "runtime/PropertyTree.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

#include "MockFileSystem.h"
#include "MockLog.h"
#include "MockNode.h"
#include "MockPool.h"
#include "MockProxy.h"
#include "MockRuntime.h"

using namespace host;
using namespace mocks;
using namespace property_tree;

namespace
{
    struct SubFixture
    {
        SubFixture( const Path& app, const Path& web )
            : idx( 0 )
        {
            MOCK_EXPECT( system.MakePaths );
            MOCK_EXPECT( system.Exists ).with( app ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( app ).returns( true );
            MOCK_EXPECT( system.IsDirectory ).with( web ).returns( true );
            MOCK_EXPECT( proxy.GetPort ).returns( 8080 );
            MOCK_EXPECT( system.MakeAnyPath ).calls( boost::bind( &SubFixture::MakePath, this, _1 ) );
        };

        Path MakePath( const Path& root )
        {
            return root / boost::lexical_cast< std::string >( ++idx );
        }

        size_t idx;
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
                                   "\"ident\":\"myName\","
                                   "\"name\":\"myName\","
                                   "\"port\":\"1337\","
                                   "\"root\":\"root\","
                                   "\"process\":{\"pid\":\"1234\",\"name\":\"e:\\/java\\/some_java.exe\"}"
                                   "}";

    const std::string idIdleText = "87654321-4321-4321-4321-cba987654321";
    const Uuid idIdle = boost::uuids::string_generator()( idIdleText );
    const std::string nodeIdle = "{"
                                 "\"id\":\"" + idIdleText + "\","
                                 "\"ident\":\"myName2\","
                                 "\"name\":\"myName2\","
                                 "\"port\":\"1338\","
                                 "\"root\":\"root\""
                                 "}";

    template< bool isCluster = false >
    struct Fixture
    {
        Fixture()
            : root   ( "e:/root" )
            , app    ( "e:/bin/node.exe" )
            , web    ( "e:/zomg/www" )
            , type   ( isCluster ? "cluster" : "node" )
            , sub    ( app, web )
            , control( sub.log, sub.runtime, sub.system, sub.nodes, root, app, web, type, 0, sub.pool, sub.proxy )
        {
            // NOTHING
        }
        const Path root;
        const Path app;
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
                MOCK_EXPECT( sub.nodes.Make5 ).once().with( mock::any, mock::any, tree.get< std::string >( "name" ), mock::any, mock::any ).returns( node );
                MOCK_EXPECT( node->Start ).once().returns( true );
            }
            else
            {
                MOCK_EXPECT( sub.nodes.Make1 ).once().returns( node );
            }
            MOCK_EXPECT( sub.system.WriteFile ).returns( true );
            MOCK_EXPECT( sub.proxy.Register ).once().with( node->GetIdent(), "localhost", node->GetPort() );
            MOCK_EXPECT( node->Stop ).once().returns( true );
            return node;
        }

        void Reload()
        {
            MOCK_EXPECT( sub.system.Walk ).once().with( root / "nodes", false ).returns( boost::assign::list_of< Path >( "a" )( "b" ) );
            MOCK_EXPECT( sub.system.IsFile ).once().with( "a/node.id" ).returns( true );
            MOCK_EXPECT( sub.system.IsFile ).once().with( "b/node.id" ).returns( true );
            active = AddNode( idActive, nodeActive, "a/node.id" );
            idle = AddNode( idIdle, nodeIdle, "b/node.id" );
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
    NodeController::T_Node node = control.Create( "myName2", "myName2", 16, 8 );
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

    MOCK_EXPECT( sub.proxy.Unregister ).once().with( active->GetIdent() );
    MOCK_RESET( active->Stop );
    MOCK_EXPECT( active->Remove ).once();
    NodeController::T_Node node = control.Delete( idActive );
    BOOST_CHECK_EQUAL( node->GetId(), idActive );
    BOOST_CHECK( !control.Has( idActive ) );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );

    MOCK_EXPECT( sub.proxy.Unregister ).once().with( idle->GetIdent() );
    MOCK_RESET( idle->Stop );
    MOCK_EXPECT( idle->Remove ).once();
    node = control.Delete( idIdle );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 0 ) );
}

BOOST_FIXTURE_TEST_CASE( node_controller_starts, Fixture<> )
{
    Reload();
    MOCK_EXPECT( idle->Start ).once().returns( true );
    NodeController::T_Node node = control.Start( idIdle );
    BOOST_CHECK_EQUAL( node->GetId(), idIdle );
}

BOOST_FIXTURE_TEST_CASE( node_controller_stops, Fixture<> )
{
    Reload();
    MOCK_EXPECT( active->Stop ).once().returns( true );
    NodeController::T_Node node = control.Stop( idActive );
    BOOST_CHECK_EQUAL( node->GetId(), idActive );
}

BOOST_FIXTURE_TEST_CASE( node_controller_upload_cache, Fixture<> )
{
    Reload();
    std::istringstream stream;
    MOCK_EXPECT( idle->UploadCache ).once().with( boost::ref( stream ) );
    MOCK_EXPECT( idle->GetCache ).once().returns( Tree() );
    Tree tree = control.UploadCache( idIdle, stream );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_get_cache, Fixture<> )
{
    Reload();
    MOCK_EXPECT( idle->GetCache ).once().returns( Tree() );
    Tree tree = control.GetCache( idIdle );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_delete_cache, Fixture<> )
{
    Reload();
    MOCK_EXPECT( idle->DeleteCache ).once().returns( Tree() );
    Tree tree = control.DeleteCache( idIdle );
    BOOST_CHECK_EQUAL( ToJson( tree ), "{}" );
}

BOOST_FIXTURE_TEST_CASE( node_controller_cannot_create_node_with_duplicate_ident, Fixture<> )
{
    Reload();
    const std::string idText = "84003698-7894-3549-0973-454970358210";
    const Uuid id = boost::uuids::string_generator()( idText );
    Tree tree;
    tree.put( "ident", "myName" );
    tree.put( "port", 4567 );
    boost::shared_ptr< MockNode > node = boost::make_shared< MockNode >( id, tree );
    MOCK_EXPECT( sub.nodes.Make5 ).once().returns( node );
    control.Create( "myName", "zomg", 16, 8 );
    MOCK_EXPECT( sub.system.Remove ).once().returns( true );
}
