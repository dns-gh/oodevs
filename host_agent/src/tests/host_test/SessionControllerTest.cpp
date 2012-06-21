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
#include <host/SessionController.h>
#include <runtime/Utf8.h>

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

using namespace host;
using runtime::Utf8Convert;
using mocks::MockFileSystem;
using mocks::MockLog;
using mocks::MockNodeController;
using mocks::MockPool;
using mocks::MockRuntime;
using mocks::MockSession;
using mocks::MockSessionFactory;

namespace
{
    std::string GetApp( const Path& apps, bool replace = true )
    {
        std::string reply = Utf8Convert( apps / "simulation_app.exe" );
        if( replace )
            std::replace( reply.begin(), reply.end(), '\\', '/' );
        return reply;
    }

    struct SubFixture
    {
        SubFixture( const Path& root, const Path& apps )
            : nodes( false )
            , idx  ( 0 )
        {
            MOCK_EXPECT( system.MakePaths ).with( root / "sessions" );
            MOCK_EXPECT( system.IsDirectory ).with( root ).returns( true );
            MOCK_EXPECT( system.IsDirectory ).with( apps ).returns( true );
            MOCK_EXPECT( system.Exists ).with( GetApp( apps ) ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( GetApp( apps ) ).returns( true );
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
        MockPool pool;
        MockSessionFactory factory;
        MockNodeController nodes;
    };

    const std::string idNodeText = "56789abc-1234-1234-1234-123412345678";
    const Uuid idNode = boost::uuids::string_generator()( idNodeText );

    const std::string idActiveText = "12345678-1234-1234-1234-123456789abc";
    const Uuid idActive = boost::uuids::string_generator()( idActiveText );
    const std::string sessionActive = "{"
                                   "\"id\":\"" + idActiveText + "\","
                                   "\"node\":\"" + idNodeText + "\","
                                   "\"name\":\"myName\","
                                   "\"exercise\":\"myExercise\","
                                   "\"port\":\"1337\","
                                   "\"status\":\"playing\","
                                   "\"process\":{\"pid\":\"1234\",\"name\":\"e:\\/apps\\/simulation_app.exe\"}"
                                   "}";

    const std::string idIdleText = "87654321-4321-4321-4321-cba987654321";
    const Uuid idIdle = boost::uuids::string_generator()( idIdleText );
    const std::string sessionIdle = "{"
                                 "\"id\":\"" + idIdleText + "\","
                                 "\"node\":\"" + idNodeText + "\","
                                 "\"name\":\"myName2\","
                                 "\"exercise\":\"myExercise2\","
                                 "\"port\":\"1338\","
                                 "\"status\":\"stopped\""
                                 "}";

    bool IsKnownNode( const Session_ABC& session )
    {
        return session.GetNode() == idNode;
    }

    struct Fixture
    {
        Fixture()
            : root   ( "e:/root" )
            , apps   ( "e:/apps" )
            , sub    ( root, apps )
            , control( sub.log, sub.runtime, sub.system, sub.factory, sub.nodes, root, apps, sub.pool )
        {
            // NOTHING
        }
        const Path root;
        const Path apps;
        SubFixture sub;
        SessionController control;
        boost::shared_ptr< MockSession > active;
        boost::shared_ptr< MockSession > idle;

        boost::shared_ptr< MockSession > AddSession( const Uuid& id, const Uuid& node, const std::string& text, const Path& path = Path() )
        {
            const Tree tree = FromJson( text );
            boost::shared_ptr< MockSession > session = boost::make_shared< MockSession >( id, node, tree );
            if( path.empty() )
            {
                MOCK_EXPECT( sub.factory.Make4 ).once().with( mock::any, node, session->GetName(), session->GetExercise() ).returns( session );
                MOCK_EXPECT( session->Start ).once().returns( true );
            }
            else
            {
                MOCK_EXPECT( sub.factory.Make1 ).once().returns( session );
            }
            MOCK_EXPECT( sub.system.WriteFile ).returns( true );
            MOCK_EXPECT( session->Stop ).once().returns( true );
            return session;
        }

        void Reload()
        {
            MOCK_EXPECT( sub.system.Glob ).once().with( root / "sessions", "session.id" ).returns( boost::assign::list_of< Path >( "a/b/c/session.id" )( "session.id" ) );
            active = AddSession( idActive, idNode, sessionActive, "a/b/c/session.id" );
            idle = AddSession( idIdle, idNode, sessionIdle, "session.id" );
            control.Reload( &IsKnownNode );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( session_controller_reloads, Fixture )
{
    Reload();
    BOOST_CHECK_EQUAL( control.Count(), size_t( 2 ) );
    SessionController::T_Sessions sessions = control.List();
    BOOST_CHECK_EQUAL( sessions.size(), size_t( 2 ) );
    BOOST_CHECK( control.Get( idActive ) );
    BOOST_CHECK( control.Get( idIdle ) );
}

BOOST_FIXTURE_TEST_CASE( session_controller_creates, Fixture )
{
    AddSession( idIdle, idNode, sessionIdle );
    SessionController::T_Session session = control.Create( idNode, "myName2", "myExercise2" );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );
    BOOST_CHECK( control.Has( idIdle ) );
    BOOST_CHECK_EQUAL( control.Get( idIdle ), session );
    SessionController::T_Sessions sessions = control.List();
    BOOST_REQUIRE_EQUAL( sessions.size(), size_t( 1 ) );
    BOOST_CHECK_EQUAL( sessions.front(), session );
}

BOOST_FIXTURE_TEST_CASE( session_controller_deletes, Fixture )
{
    Reload();

    MOCK_EXPECT( sub.system.Remove ).exactly( 2 ).returns( true );
    MOCK_EXPECT( active->Unlink ).once();
    SessionController::T_Session session = control.Delete( idActive );
    BOOST_CHECK_EQUAL( session->GetId(), idActive );
    BOOST_CHECK( !control.Has( idActive ) );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );

    MOCK_EXPECT( sub.system.Remove ).exactly( 2 ).returns( true );
    MOCK_EXPECT( idle->Unlink ).once();
    session = control.Delete( idIdle );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 0 ) );
}

BOOST_FIXTURE_TEST_CASE( session_controller_starts, Fixture )
{
    Reload();
    MOCK_EXPECT( idle->Start ).once().returns( true );
    SessionController::T_Session session = control.Start( idIdle );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
}

BOOST_FIXTURE_TEST_CASE( session_controller_stops, Fixture )
{
    Reload();
    MOCK_EXPECT( active->Stop ).once().returns( true );
    SessionController::T_Session session = control.Stop( idActive );
    BOOST_CHECK_EQUAL( session->GetId(), idActive );
}
