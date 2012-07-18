// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/Session.h"
#include "runtime/Event.h"
#include "runtime/Pool.h"
#include "runtime/PropertyTree.h"

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid//uuid_io.hpp>

#include "MockClient.h"
#include "MockFileSystem.h"
#include "MockNode.h"
#include "MockPortFactory.h"
#include "MockProcess.h"
#include "MockRuntime.h"

using namespace host;
using namespace mocks;
using namespace property_tree;
using namespace runtime;

struct host::node::Token
{
    Token() {}
};

namespace
{
    const std::string defaultIdString = "12345678-90AB-CDEF-9876-543210123456";
    const Uuid defaultId = boost::uuids::string_generator()( defaultIdString );
    const std::string defaultNodeString = "10123456-CDEF-9876-90AB-543212345678";
    const Uuid defaultNode = boost::uuids::string_generator()( defaultNodeString );
    const std::string defaultName = "myName";
    const std::string defaultExercise = "MyExercise";
    const int defaultPort = 1337;
    const int processPid = 7331;
    const std::string processName = "myProcessName";
    const std::string links = "{"
        "\"exercise\":{\"name\":\"a\",\"root\":\"b\",\"checksum\":\"c\"},"
        "\"terrain\":{\"name\":\"a\",\"root\":\"b\",\"checksum\":\"c\"},"
        "\"model\":{\"name\":\"a\",\"root\":\"b\",\"checksum\":\"c\"}"
        "}";
    const std::string items =
        "\"exercise\":{\"name\":\"a\",\"checksum\":\"c\"},"
        "\"terrain\":{\"name\":\"a\",\"checksum\":\"c\"},"
        "\"model\":{\"name\":\"a\",\"checksum\":\"c\"}";

    int BlockUntil( Event& start, Event& end, int code )
    {
        start.Signal();
        end.Wait();
        return code;
    }

    std::string GetState( Session_ABC& session )
    {
        const Tree data = session.GetProperties();
        return data.get< std::string >( "status" );
    }

    struct Fixture
    {
        typedef boost::shared_ptr< Session > SessionPtr;
        typedef boost::shared_ptr< MockProcess > ProcessPtr;
        MockClient client;
        MockFileSystem system;
        MockRuntime runtime;
        MockPortFactory ports;
        boost::shared_ptr< MockNode > node;
        const host::Path apps;
        Fixture()
            : node ( boost::make_shared< MockNode >( defaultNode, FromJson( "{\"name\":\"a\",\"port\":\"1\"}" ) ) )
            , apps ( "apps" )
        {
            MOCK_EXPECT( node->SessionStart ).returns( boost::make_shared< host::node::Token >() );
        }

        SessionPtr MakeSession()
        {
            MOCK_EXPECT( node->LinkExerciseName ).once().with( defaultExercise ).returns( FromJson( links ) );
            return boost::make_shared< Session >( system, "", node, client, defaultId, defaultName, defaultExercise, Port( new MockPort( defaultPort ) ) );
        }

        SessionPtr ReloadSession( const Tree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            const Tree data = FromJson( links );
            MOCK_EXPECT( node->LinkExerciseTree ).once().with( data ).returns( data );
            return boost::make_shared< Session >( system, "", node, client, tree, runtime, ports );
        }

        ProcessPtr StartSession( Session& session, int pid, const std::string& name )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( runtime.Start ).once().returns( process );
            MOCK_EXPECT( system.MakePaths ).once();
            MOCK_EXPECT( system.WriteFile ).once().returns( true );
            BOOST_REQUIRE( session.Start( runtime, apps ) );
            return process;
        }

        void ExpectWebRequest( const std::string& url, int code )
        {
            boost::shared_ptr< MockResponse > rpy = boost::make_shared< MockResponse >();
            MOCK_EXPECT( rpy->GetStatus ).returns( code );
            MOCK_EXPECT( client.Get ).once().with( "localhost", mock::any, url, mock::any ).returns( rpy );
        }

        boost::shared_ptr< MockResponse > ExpectBlockingWebRequest( const std::string& url, int code, Event& start, Event& end )
        {
            boost::shared_ptr< MockResponse > rpy = boost::make_shared< MockResponse >();
            MOCK_EXPECT( client.Get ).once().with( "localhost", mock::any, url, mock::any ).returns( rpy );
            MOCK_EXPECT( rpy->GetStatus ).calls( boost::bind( &BlockUntil, boost::ref( start ), boost::ref( end ), code ) );
            return rpy;
        }

        void StopSession( Session& session, ProcessPtr process = ProcessPtr() )
        {
            if( process )
            {
                ExpectWebRequest( "/stop", 200 );
                MOCK_EXPECT( process->Join ).returns( true );
            }
            BOOST_CHECK( session.Stop() );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( session_constructs, Fixture )
{
    SessionPtr session = MakeSession();
    BOOST_CHECK_EQUAL( session->GetId(), defaultId );
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_stops, Fixture )
{
    SessionPtr session = MakeSession();
    ProcessPtr process = StartSession( *session, processPid, processName );
    StopSession( *session, process );
    BOOST_CHECK( !session->Stop() );
}

BOOST_FIXTURE_TEST_CASE( session_converts, Fixture )
{
    SessionPtr session = MakeSession();
    const std::string base = "{"
        "\"id\":\"12345678-90ab-cdef-9876-543210123456\","
        "\"node\":\"10123456-cdef-9876-90ab-543212345678\","
        "\"name\":\"myName\","
        "\"port\":\"1337\",";
    BOOST_CHECK_EQUAL( ToJson( session->GetProperties() ), base +
        "\"status\":\"stopped\""
        "," + items +
        "}" );
    BOOST_CHECK_EQUAL( ToJson( session->Save() ), base +
        "\"status\":\"stopped\""
        ",\"links\":" + links +
        "}" );
    ProcessPtr process = StartSession( *session, processPid, processName );
    BOOST_CHECK_EQUAL( ToJson( session->GetProperties() ), base +
        "\"status\":\"playing\""
        "," + items +
        "}" );
    BOOST_CHECK_EQUAL( ToJson( session->Save() ), base +
        "\"status\":\"playing\","
        "\"links\":" + links + ","
        "\"process\":{"
            "\"pid\":\"7331\","
            "\"name\":\"myProcessName\""
        "}}" );
    StopSession( *session, process );
    BOOST_CHECK_EQUAL( ToJson( session->GetProperties() ), base +
        "\"status\":\"stopped\""
        "," + items +
        "}" );
    BOOST_CHECK_EQUAL( ToJson( session->Save() ), base +
        "\"status\":\"stopped\""
        ",\"links\":" + links +
        "}" );
}

BOOST_FIXTURE_TEST_CASE( session_reloads, Fixture )
{
    const Tree save = MakeSession()->Save();
    ReloadSession( save );
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_reloads, Fixture )
{
    SessionPtr session = MakeSession();
    ProcessPtr process = StartSession( *session, processPid, processName );
    const Tree save = session->Save();
    session = ReloadSession( save, process );
    StopSession( *session, process );
}

BOOST_FIXTURE_TEST_CASE( session_rejects_bind_to_another_process, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    const Tree save = session->Save();
    session = ReloadSession( save, boost::make_shared< MockProcess >( processPid, processName + "_" ) );
    BOOST_CHECK( !session->Stop() );
}

BOOST_FIXTURE_TEST_CASE( session_can_start_twice, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    BOOST_CHECK( !session->Start( runtime, apps ) );
}

BOOST_FIXTURE_TEST_CASE( session_can_pause_and_restart, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    ExpectWebRequest( "/pause", 200 );
    BOOST_CHECK( session->Pause() );
    BOOST_CHECK( !session->Pause() );
    ExpectWebRequest( "/play", 200 );
    BOOST_CHECK( session->Start( runtime, apps ) );
}

BOOST_FIXTURE_TEST_CASE( session_discards_outdated_updates_due_to_invalidated_process, Fixture )
{
    SessionPtr session = MakeSession();
    ProcessPtr process = StartSession( *session, processPid, processName );
    Pool pool( 2 );

    Event waitPause, endPause;
    ExpectBlockingWebRequest( "/pause", 200, waitPause, endPause );
    Pool_ABC::Future pause = pool.Go( boost::bind( &Session_ABC::Pause, session ) );
    waitPause.Wait();

    Event waitPoll, endPoll;
    boost::shared_ptr< MockResponse > rpy = ExpectBlockingWebRequest( "/get", 200, waitPoll, endPoll );
    MOCK_EXPECT( rpy->GetBody ).once().returns( "{\"state\":\"paused\"}" );
    Pool_ABC::Future poll = pool.Go( boost::bind( &Session_ABC::Poll, session ) );
    waitPoll.Wait();

    StopSession( *session, process );
    BOOST_CHECK_EQUAL( GetState( *session ), "stopped" );

    endPause.Signal();
    pause.wait();
    BOOST_CHECK_EQUAL( GetState( *session ), "stopped" );

    endPoll.Signal();
    poll.wait();
    BOOST_CHECK_EQUAL( GetState( *session ), "stopped" );
}

BOOST_FIXTURE_TEST_CASE( session_discard_outdated_updates_due_to_invalidated_counter, Fixture )
{
    SessionPtr session = MakeSession();
    ProcessPtr process = StartSession( *session, processPid, processName );
    Pool pool( 1 );

    Event waitPause, endPause;
    ExpectBlockingWebRequest( "/pause", 200, waitPause, endPause );
    Pool_ABC::Future pause = pool.Go( boost::bind( &Session_ABC::Pause, session ) );
    waitPause.Wait();

    ExpectWebRequest( "/pause", 200 );
    session->Pause();
    ExpectWebRequest( "/play", 200 );
    session->Start( runtime, apps );
    BOOST_CHECK_EQUAL( GetState( *session ), "playing" );

    endPause.Signal();
    pause.wait();
    BOOST_CHECK_EQUAL( GetState( *session ), "playing" );
}