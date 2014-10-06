// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test_pch.h"

#include "host/SessionController.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Configs.h"
#include "web/User.h"
#include "web/HttpException.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

#include "MockFileSystem.h"
#include "MockLog.h"
#include "MockNodeController.h"
#include "MockPool.h"
#include "MockRuntime.h"
#include "MockSession.h"

using namespace host;
using namespace mocks;
using namespace property_tree;
using runtime::Utf8;

namespace
{
    struct SubFixture
    {
        SubFixture( const Path& root, const Path& simulation, const Path& replayer, const Path& timeline )
            : nodes( false )
            , idx  ( 0 )
        {
            MOCK_EXPECT( fs.MakePaths ).with( root / "sessions" / "_" );
            MOCK_EXPECT( fs.IsDirectory ).with( root ).returns( true );
            MOCK_EXPECT( fs.IsFile ).with( simulation ).returns( true );
            MOCK_EXPECT( fs.IsFile ).with( replayer ).returns( true );
            MOCK_EXPECT( fs.IsFile ).with( timeline ).returns( true );
            MOCK_EXPECT( fs.MakeAnyPath ).calls( boost::bind( &SubFixture::MakePath, this, _1 ) );
            MOCK_EXPECT( fs.Remove ).returns( true );
        };

        Path MakePath( const Path& root )
        {
            return root / boost::lexical_cast< std::string >( ++idx );
        }

        size_t idx;
        MockLog log;
        MockRuntime runtime;
        MockFileSystem fs;
        MockPool pool;
        MockSessionFactory factory;
        MockNodeController nodes;
    };

    const std::string idNodeText = "56789abc-1234-1234-1234-123412345678";
    const Uuid idNode = boost::uuids::string_generator()( idNodeText );
    const web::User standardUser = web::User( 42, "standard", web::USER_TYPE_ADMINISTRATOR, idNode );

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
            : root      ( "e:/root" )
            , simulation( "e:/apps/sim.exe" )
            , replayer  ( "e:/apps/sim.exe" )
            , timeline  ( "e:/apps/timeline.exe" )
            , sub       ( root, simulation, replayer, timeline )
            , control   ( sub.log, sub.runtime, sub.fs, sub.factory, sub.nodes, root, simulation, replayer, timeline, sub.pool )
        {
            // NOTHING
        }
        const Path root;
        const Path simulation;
        const Path replayer;
        const Path timeline;
        SubFixture sub;
        SessionController control;
        boost::shared_ptr< MockSession > active;
        boost::shared_ptr< MockSession > idle;

        boost::shared_ptr< MockSession > AddSession( const Uuid& id, const web::User& user, const std::string& text, const Path& path = Path() )
        {
            const Tree tree = FromJson( text );
            boost::shared_ptr< MockSession > session = boost::make_shared< MockSession >( id, user.node, tree );
            if( path.empty() )
                MOCK_EXPECT( sub.factory.Make5 ).once().with( mock::any, mock::any, user.node, mock::any, session->GetExercise(), mock::same( user ) ).returns( session );
            else
                MOCK_EXPECT( sub.factory.Make2 ).once().returns( session );
            MOCK_EXPECT( sub.fs.WriteFile ).returns( true );
            MOCK_EXPECT( session->Stop ).once().returns( true );
            return session;
        }

        void Reload()
        {
            MOCK_EXPECT( sub.fs.Walk ).once().with( root / "sessions", false,
                boost::bind( &MockFileSystem::Apply, &sub.fs, _1, boost::assign::list_of< Path >( "a" )( "b" ) ) );
            MOCK_EXPECT( sub.fs.IsFile ).once().with( "a/session.id" ).returns( true );
            MOCK_EXPECT( sub.fs.IsFile ).once().with( "b/session.id" ).returns( true );
            active = AddSession( idActive, standardUser, sessionActive, "a/session.id" );
            idle = AddSession( idIdle, standardUser, sessionIdle, "b/session.id" );
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
    BOOST_CHECK( control.Get( idNode, idActive ) );
    BOOST_CHECK( control.Get( idNode, idIdle ) );
}

BOOST_FIXTURE_TEST_CASE( session_controller_creates, Fixture )
{
    AddSession( idIdle, standardUser, sessionIdle );
    web::session::Config cfg;
    cfg.name = "myName2";
    SessionController::T_Session session = control.Create( standardUser, cfg, "myExercise2" );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );
    BOOST_CHECK_EQUAL( control.Get( idNode, idIdle ), session );
    SessionController::T_Sessions sessions = control.List();
    BOOST_REQUIRE_EQUAL( sessions.size(), size_t( 1 ) );
    BOOST_CHECK_EQUAL( sessions.front(), session );
}

BOOST_FIXTURE_TEST_CASE( session_controller_deletes, Fixture )
{
    Reload();

    MOCK_RESET( active->Stop );
    MOCK_EXPECT( active->Remove ).once();
    MOCK_EXPECT( idle->DetachReplay ).once().with( mock::same( *active ) );
    SessionController::T_Session session = control.Delete( idNode, idActive );
    BOOST_CHECK_EQUAL( session->GetId(), idActive );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );

    MOCK_RESET( idle->Stop );
    MOCK_EXPECT( idle->Remove ).once();
    session = control.Delete( idNode, idIdle );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 0 ) );
}

BOOST_FIXTURE_TEST_CASE( session_controller_starts, Fixture )
{
    Reload();
    const std::string checkpoint = "checkpoint";
    MOCK_EXPECT( idle->Start ).once().with( mock::any, mock::any, checkpoint ).returns( true );
    MOCK_EXPECT( idle->IsAuthorized ).once().with( mock::same( standardUser ) ).returns( true );
    SessionController::T_Session session = control.Start( standardUser, idIdle, checkpoint );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
}

BOOST_FIXTURE_TEST_CASE( unauthorized_agent_can_not_start_session, Fixture )
{
    Reload();
    MOCK_EXPECT( idle->IsAuthorized ).once().with( mock::same( standardUser ) ).returns( false );
    BOOST_CHECK_THROW( control.Start( standardUser, idIdle, "checkpoint" ), web::HttpException ); ;
}

BOOST_FIXTURE_TEST_CASE( session_controller_stops, Fixture )
{
    Reload();
    MOCK_EXPECT( active->Stop ).once().returns( true );
    MOCK_EXPECT( active->IsAuthorized ).once().with( mock::same( standardUser ) ).returns( true );
    SessionController::T_Session session = control.Stop( standardUser, idActive );
    BOOST_CHECK_EQUAL( session->GetId(), idActive );
}

BOOST_FIXTURE_TEST_CASE( session_controller_starts_with_right_app, Fixture )
{
    Reload();
    MOCK_EXPECT( idle->Start ).once().with( simulation, timeline, mock::any ).returns( true );
    MOCK_EXPECT( idle->IsAuthorized ).once().with( mock::same( standardUser ) ).returns( true );
    control.Start( standardUser, idIdle, std::string() );
    MOCK_RESET( idle->IsReplay );
    MOCK_EXPECT( idle->IsReplay ).once().returns( true );
    MOCK_EXPECT( idle->IsAuthorized ).once().with( mock::same( standardUser ) ).returns( true );
    MOCK_EXPECT( idle->Start ).once().with( replayer, timeline, mock::any ).returns( true );
    control.Start( standardUser, idIdle, std::string() );
}
