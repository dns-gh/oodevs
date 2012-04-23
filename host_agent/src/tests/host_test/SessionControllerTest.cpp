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
#include <host/Json.h>
#include <host/SessionController.h>
#include <runtime/Utf8.h>

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
using mocks::MockPool;
using mocks::MockPort;
using mocks::MockPortFactory;
using mocks::MockProcess;

namespace
{
    std::string GetApp( const std::string& apps, bool replace = true )
    {
        std::string reply = runtime::Utf8Convert( boost::filesystem::path( apps ) / "simulation_app.exe" );
        if( replace )
            std::replace( reply.begin(), reply.end(), '\\', '/' );
        return reply;
    }

    struct SubFixture
    {
        SubFixture( const std::string& data, const std::string& apps )
        {
            MOCK_EXPECT( system.IsDirectory ).with( data ).returns( true );
            MOCK_EXPECT( system.IsDirectory ).with( apps ).returns( true );
            MOCK_EXPECT( system.Exists ).with( GetApp( apps ) ).returns( true );
            MOCK_EXPECT( system.IsFile ).with( GetApp( apps ) ).returns( true );
            MOCK_EXPECT( system.Glob ).with( data + "/exercises", L"exercise.xml" ).returns( boost::assign::list_of< std::string >
                ( "e:/data/exercises/baroud/exercise.xml" )
                ( "e:/data/exercises/test/musoria/exercise.xml" )
            );
        };
        MockLog log;
        MockRuntime runtime;
        MockFileSystem system;
        MockUuidFactory uuids;
        MockPool pool;
        MockPortFactory ports;
    };

    const std::string idNodeText = "56789abc-1234-1234-1234-123412345678";
    const boost::uuids::uuid idNode = boost::uuids::string_generator()( idNodeText );

    const std::string idActiveText = "12345678-1234-1234-1234-123456789abc";
    const boost::uuids::uuid idActive = boost::uuids::string_generator()( idActiveText );
    const std::string sessionActive = "{"
                                   "\"id\":\"" + idActiveText + "\","
                                   "\"node\":\"" + idNodeText + "\","
                                   "\"name\":\"myName\","
                                   "\"exercise\":\"myExercise\","
                                   "\"port\":\"1337\","
                                   "\"status\":\"running\","
                                   "\"process\":{\"pid\":\"1234\",\"name\":\"e:/apps/simulation_app.exe\"}"
                                   "}";

    const std::string idIdleText = "87654321-4321-4321-4321-cba987654321";
    const boost::uuids::uuid idIdle = boost::uuids::string_generator()( idIdleText );
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
            : data   ( "e:/data" )
            , apps   ( "e:/apps" )
            , sub    ( data, apps )
            , control( sub.log, sub.runtime, sub.system, sub.uuids, data, apps, sub.pool, sub.ports )
        {
            // NOTHING
        }
        const std::string data;
        const std::string apps;
        SubFixture sub;
        SessionController control;

        boost::shared_ptr< MockProcess > Reload()
        {
            MOCK_EXPECT( sub.system.Glob ).once().with( "e:/data/exercises", L"session.id" ).returns( boost::assign::list_of< boost::filesystem::path >( "a/b/c/session.id" )( "session.id" ) );
            MOCK_EXPECT( sub.system.ReadFile ).once().with( "a/b/c/session.id" ).returns( sessionActive );
            MOCK_EXPECT( sub.ports.Create1 ).once().with( 1337 ).returns( new MockPort( 1337 ) );
            const boost::filesystem::path app = apps;
            boost::shared_ptr< MockProcess > process = boost::make_shared< MockProcess >( 1234, "e:/apps/simulation_app.exe" );
            MOCK_EXPECT( sub.runtime.GetProcess ).once().with( 1234 ).returns( process );
            MOCK_EXPECT( sub.system.ReadFile ).once().with( "session.id" ).returns( sessionIdle );
            MOCK_EXPECT( sub.ports.Create1 ).once().with( 1338 ).returns( new MockPort( 1338 ) );
            control.Reload( &IsKnownNode );
            return process;
        }
    };

    bool CheckParameters( const std::vector< std::string >& actual, const std::vector< std::string >& expected )
    {
        if( actual.size() != expected.size() )
            return false;
        for( std::vector< std::string >::const_iterator a = actual.begin(), b = expected.begin(); a < actual.end(); ++a, ++b )
        {
            if( a->size() != b->size() )
                return false;
            std::string ra = *a;
            std::replace( ra.begin(), ra.end(), '\\', '/' );
            if( ra != *b )
                return false;
        }
        return true;
    }
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
    MOCK_EXPECT( sub.ports.Create0 ).once().returns( new MockPort( 1337 ) );
    MOCK_EXPECT( sub.uuids.Create ).once().returns( idIdle );
    MOCK_EXPECT( sub.runtime.Start ).once().with( GetApp( apps, false ), boost::bind( &CheckParameters, _1, boost::assign::list_of< std::string >
        ( "--root-dir \"e:/data\"" )
        ( "--exercises-dir \"e:/data/exercises\"" )
        ( "--terrains-dir \"e:/data/data/terrains\"" )
        ( "--models-dir \"e:/data/data/models\"" )
        ( "--exercise \"baroud\"" )
        ( "--session \"" + idIdleText + "\"" ) ),
        "e:/apps" ).returns( boost::make_shared< MockProcess >( 1377, GetApp( apps ) ) );
    MOCK_EXPECT( sub.system.WriteFile ).exactly( 2 );
    MOCK_EXPECT( sub.system.MakeDirectory ).once().with( "e:/data/exercises/baroud/sessions/" + idIdleText );
    SessionController::T_Session session = control.Create( idNode, "zebulon", "baroud" );
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
    boost::shared_ptr< MockProcess > process = Reload();
    MOCK_EXPECT( process->Kill ).once().returns( true );
    MOCK_EXPECT( sub.system.Remove );
    SessionController::T_Session session = control.Delete( idActive );
    BOOST_CHECK_EQUAL( session->GetId(), idActive );
    BOOST_CHECK( !control.Has( idActive ) );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 1 ) );
    session = control.Delete( idIdle );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
    BOOST_CHECK_EQUAL( control.Count(), size_t( 0 ) );
}

BOOST_FIXTURE_TEST_CASE( session_controller_starts, Fixture )
{
    Reload();
    MOCK_EXPECT( sub.system.WriteFile ).once();
    MOCK_EXPECT( sub.runtime.Start ).once().returns( boost::make_shared< MockProcess >( 1398, GetApp( apps ) ) );
    SessionController::T_Session session = control.Start( idIdle );
    BOOST_CHECK_EQUAL( session->GetId(), idIdle );
}

BOOST_FIXTURE_TEST_CASE( session_controller_stops, Fixture )
{
    boost::shared_ptr< MockProcess > ptr = Reload();
    MOCK_EXPECT( sub.system.WriteFile ).once();
    MOCK_EXPECT( ptr->Kill ).once().returns( true );
    SessionController::T_Session session = control.Stop( idActive );
    BOOST_CHECK_EQUAL( session->GetId(), idActive );
}
