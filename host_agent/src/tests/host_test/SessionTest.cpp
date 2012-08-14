// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "TreeHelpers.h"
#include "host/Session.h"
#include "runtime/Event.h"
#include "runtime/Pool.h"
#include "runtime/PropertyTree.h"
#include "web/Configs.h"

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid//uuid_io.hpp>
#include <boost/xpressive/xpressive.hpp>

#include "MockClient.h"
#include "MockFileSystem.h"
#include "MockNode.h"
#include "MockPool.h"
#include "MockPortFactory.h"
#include "MockProcess.h"
#include "MockRuntime.h"
#include "MockUuidFactory.h"

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

    host::Path MakePath( int& idx, const host::Path& root )
    {
        return root / boost::lexical_cast< std::string >( ++idx );
    }

    struct Fixture
    {
        typedef boost::shared_ptr< Session > SessionPtr;
        typedef boost::shared_ptr< MockProcess > ProcessPtr;
        MockClient client;
        MockFileSystem system;
        MockRuntime runtime;
        MockPortFactory ports;
        MockPool pool;
        MockUuidFactory uuids;
        boost::shared_ptr< MockNode > node;
        const host::Path apps;
        int any_idx;
        Fixture()
            : node   ( boost::make_shared< MockNode >( defaultNode, FromJson( "{\"ident\":\"a\",\"name\":\"a\",\"port\":\"1\"}" ) ) )
            , apps   ( "apps" )
            , any_idx( 0 )
        {
            MOCK_EXPECT( system.GetDirectorySize ).returns( 0 );
            MOCK_EXPECT( system.IsDirectory ).returns( false) ;
            MOCK_EXPECT( system.MakePaths );
            MOCK_EXPECT( system.MakeAnyPath ).calls( boost::bind( &MakePath, boost::ref( any_idx ), _1 ) );
            MOCK_EXPECT( system.Walk );
            MOCK_EXPECT( system.Rename ).returns( true );
            MOCK_EXPECT( system.Remove ).returns( true );
            MOCK_EXPECT( node->StartSession ).returns( boost::make_shared< host::node::Token >() );
            MOCK_EXPECT( node->UpdateSessionSize );
        }

        SessionPtr MakeSession()
        {
            MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
            MOCK_EXPECT( ports.Create0 ).once().returns( new MockPort( defaultPort ) );
            MOCK_EXPECT( node->LinkExerciseName ).once().with( defaultExercise ).returns( FromJson( links ) );
            web::session::Config cfg;
            cfg.name = defaultName;
            SessionPaths paths( "a", "b" );
            SessionDependencies deps( system, runtime, uuids, client, pool, ports );
            return boost::make_shared< Session >( deps, node, paths, cfg, defaultExercise, boost::uuids::nil_uuid() );
        }

        SessionPtr ReloadSession( const Tree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            const Tree data = FromJson( links );
            MOCK_EXPECT( node->LinkExerciseTree ).once().with( data ).returns( data );
            SessionPaths paths( "a", "b" );
            SessionDependencies deps( system, runtime, uuids, client, pool, ports );
            return boost::make_shared< Session >( deps, node, paths, tree );
        }

        ProcessPtr StartSession( Session& session, int pid, const std::string& name )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( runtime.Start ).once().returns( process );
            MOCK_EXPECT( system.WriteFile ).once().returns( true );
            BOOST_REQUIRE( session.Start( apps, std::string() ) );
            return process;
        }

        void ExpectWebRequest( const std::string& url, int code )
        {
            boost::shared_ptr< MockResponse > rpy = boost::make_shared< MockResponse >();
            MOCK_EXPECT( rpy->GetStatus ).returns( code );
            MOCK_EXPECT( client.Get4 ).once().with( "localhost", mock::any, url, mock::any ).returns( rpy );
        }

        boost::shared_ptr< MockResponse > ExpectBlockingWebRequest( const std::string& url, int code, Event& start, Event& end )
        {
            boost::shared_ptr< MockResponse > rpy = boost::make_shared< MockResponse >();
            MOCK_EXPECT( client.Get4 ).once().with( "localhost", mock::any, url, mock::any ).returns( rpy );
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

namespace
{
std::string Filter( Tree& tree )
{
    tree.erase( "checkpoints" );
    tree.erase( "time" );
    tree.erase( "pathfind" );
    tree.erase( "recorder" );
    tree.erase( "rng" );
    return ToJson( tree );
}
}

BOOST_FIXTURE_TEST_CASE( session_converts, Fixture )
{
    SessionPtr session = MakeSession();

    T_Constraints base;
    base.push_back( boost::bind( &EqualValue, _1, "id", "12345678-90ab-cdef-9876-543210123456" ) );
    base.push_back( boost::bind( &EqualValue, _1, "node", "10123456-cdef-9876-90ab-543212345678" ) );
    base.push_back( boost::bind( &EqualValue, _1, "port", "1337" ) );
    base.push_back( boost::bind( &EqualValue, _1, "name", "myName" ) );

    T_Constraints stats;
    stats.push_back( boost::bind( &EqualValue, _1, "start_time", "" ) );
    stats.push_back( boost::bind( &EqualValue, _1, "current_time", "" ) );
    stats.push_back( boost::bind( &EqualValue, _1, "clients", "" ) );

    T_Constraints items;
    items.push_back( boost::bind( &EqualTree, _1, "model",    "{\"name\":\"a\",\"checksum\":\"c\"}" ) );
    items.push_back( boost::bind( &EqualTree, _1, "terrain",  "{\"name\":\"a\",\"checksum\":\"c\"}" ) );
    items.push_back( boost::bind( &EqualTree, _1, "exercise", "{\"name\":\"a\",\"checksum\":\"c\"}" ) );

    Tree src = session->GetProperties();
    Check( base, src );
    Check( stats, src );
    Check( items, src );
    EqualValue( src, "status", "stopped" );
    EqualValue( src, "first_time", "true" );

    src = session->Save();
    Check( base, src );
    EqualValue( src, "status", "stopped" );
    EqualValue( src, "first_time", "true" );
    EqualTree ( src, "links", links );
    EqualValue( src, "size", "0" );

    ProcessPtr process = StartSession( *session, processPid, processName );

    src = session->GetProperties();
    Check( base, src );
    Check( stats, src );
    Check( items, src );
    EqualValue( src, "status", "playing" );
    EqualValue( src, "first_time", "false" );

    src = session->Save();
    Check( base, src );
    EqualValue( src, "status", "playing" );
    EqualValue( src, "first_time", "false" );
    EqualTree ( src, "links", links );
    EqualValue( src, "size", "0" );
    EqualTree ( src, "process", "{\"pid\":\"7331\",""\"name\":\"myProcessName\"}" );

    StopSession( *session, process );

    src = session->GetProperties();
    Check( base, src );
    Check( stats, src );
    Check( items, src );
    EqualValue( src, "status", "stopped" );
    EqualValue( src, "first_time", "false" );

    src = session->Save();
    Check( base, src );
    EqualValue( src, "status", "stopped" );
    EqualValue( src, "first_time", "false" );
    EqualTree ( src, "links", links );
    EqualValue( src, "size", "0" );
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
    BOOST_CHECK( !session->Start( apps, std::string() ) );
}

BOOST_FIXTURE_TEST_CASE( session_can_pause_and_restart, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    ExpectWebRequest( "/pause", 200 );
    BOOST_CHECK( session->Pause() );
    BOOST_CHECK( !session->Pause() );
    ExpectWebRequest( "/play", 200 );
    BOOST_CHECK( session->Start( apps, std::string() ) );
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
    session->Start( apps, std::string() );
    BOOST_CHECK_EQUAL( GetState( *session ), "playing" );

    endPause.Signal();
    pause.wait();
    BOOST_CHECK_EQUAL( GetState( *session ), "playing" );
}
