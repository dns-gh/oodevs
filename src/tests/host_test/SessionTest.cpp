// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "TreeHelpers.h"
#include "host/Session.h"
#include "runtime/Event.h"
#include "runtime/Pool.h"
#include "runtime/PropertyTree.h"
#include "web/Configs.h"
#include "web/HttpException.h"
#include "web/Plugins.h"

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid//uuid_io.hpp>
#include <boost/xpressive/xpressive.hpp>

#include "MockClient.h"
#include "MockFileSystem.h"
#include "MockLog.h"
#include "MockNode.h"
#include "MockNodeController.h"
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
    const std::string sides =
        "{"
        "    \"sides\": {"
        "        \"no_side_objects\": \"true\","
        "        \"list\": {"
        "            \"1\": {"
        "                \"name\": \"Local militia\","
        "                \"created\": \"true\""
        "            },"
        "            \"3\": {"
        "                \"name\": \"Friend forces\","
        "                \"created\": \"true\""
        "            }"
        "        }"
        "    }"
        "}";

    bool EndWith( const std::string& suffix, const host::Path& path )
    {
        const auto regex = boost::xpressive::sregex::compile( suffix + "$" );
        return boost::xpressive::regex_search( path.string(), regex );
    }

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

    struct SubFixture
    {
        MockFileSystem fs;
        SubFixture()
        {
            MOCK_EXPECT( fs.Walk );
            MOCK_EXPECT( fs.Exists ).with( boost::bind( &EndWith, "\\.log", _1 ) ).returns( false );
        }
    };

    struct Fixture
    {
        typedef boost::shared_ptr< Session > SessionPtr;
        typedef boost::shared_ptr< MockProcess > ProcessPtr;
        SubFixture sub;
        MockFileSystem& fs;
        MockClient client;
        MockRuntime runtime;
        MockLog log;
        MockNodeController nodes;
        MockPortFactory ports;
        MockPool pool;
        MockUuidFactory uuids;
        boost::shared_ptr< MockNode > node;
        const host::Path apps;
        const host::Path timeline;
        const web::Plugins plugins;
        int any_idx;
        Fixture()
            : node    ( boost::make_shared< MockNode >( defaultNode, FromJson( "{\"ident\":\"a\",\"name\":\"a\",\"port\":\"1\"}" ) ) )
            , fs      ( sub.fs )
            , nodes   ( false )
            , apps    ( "apps" )
            , timeline( "timeline" )
            , plugins ( fs, "" )
            , any_idx ( 0 )
        {
            MOCK_RESET( node->GetExerciseProperties );
            MOCK_EXPECT( node->GetExerciseProperties ).returns( FromJson( sides ) );
            MOCK_EXPECT( fs.GetDirectorySize ).returns( 0 );
            MOCK_EXPECT( fs.IsDirectory ).returns( false) ;
            MOCK_EXPECT( fs.MakePaths );
            MOCK_EXPECT( fs.MakeAnyPath ).calls( boost::bind( &MakePath, boost::ref( any_idx ), _1 ) );
            MOCK_EXPECT( fs.Rename ).returns( true );
            MOCK_EXPECT( fs.Remove ).returns( true );
            MOCK_EXPECT( node->UpdateSessionSize );
            MOCK_EXPECT( node->FilterConfig );
        }

        SessionPtr MakeSession()
        {
            MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
            MOCK_EXPECT( ports.Create0 ).once().returns( new MockPort( defaultPort ) );
            MOCK_EXPECT( nodes.LinkExerciseName ).once().with( mock::same( *node ), defaultExercise ).returns( FromJson( links ) );
            web::session::Config cfg;
            cfg.name = defaultName;
            cfg.profiles.insert( web::session::Profile( "username", "password" ) );
            SessionPaths paths( "a", "b" );
            SessionDependencies deps( fs, runtime, plugins, nodes, uuids, log, client, pool, ports );
            return boost::make_shared< Session >( deps, node, paths, cfg, defaultExercise, boost::uuids::nil_uuid(), web::User( 42, "" ) );
        }

        typedef std::pair< ProcessPtr, ProcessPtr > T_Processes;

        SessionPtr ReloadSession( const Tree& tree, T_Processes processes = T_Processes(), bool valid = true )
        {
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( processes.first )
                MOCK_EXPECT( runtime.GetProcess ).once().with( processes.first->GetPid() ).returns( processes.first );
            if( processes.first && valid )
                MOCK_EXPECT( node->StartSession ).once().returns( boost::make_shared< host::node::Token >() );
            if( processes.second )
                MOCK_EXPECT( runtime.GetProcess ).once().with( processes.second->GetPid() ).returns( processes.second );
            const Tree data = FromJson( links );
            MOCK_EXPECT( nodes.LinkExerciseTree ).once().with( mock::same( *node ), data ).returns( data );
            SessionPaths paths( "a", "b" );
            SessionDependencies deps( fs, runtime, plugins, nodes, uuids, log, client, pool, ports );
            return boost::make_shared< Session >( deps, node, paths, tree );
        }

        bool StartSessionWith( Session& session, ProcessPtr process, const std::string& checkpoint = std::string() )
        {
            MOCK_EXPECT( fs.WriteFile ).once().returns( true );
            MOCK_EXPECT( node->StartSession ).once().returns( boost::make_shared< host::node::Token >() );
            return session.Start( apps, timeline, checkpoint );
        }

        T_Processes StartSession( Session& session, int pid, const std::string& name, const std::string& checkpoint = std::string() )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( runtime.Start ).once().returns( process );
            MOCK_EXPECT( ports.WaitConnected ).once().returns( true );
            MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
            MOCK_EXPECT( fs.WriteFile ).once().returns( true );
            ProcessPtr timeline = boost::make_shared< MockProcess >( processPid*2, "timeline" );
            MOCK_EXPECT( runtime.Start ).once().returns( timeline );
            BOOST_REQUIRE( StartSessionWith( session, process, checkpoint ) );
            return std::make_pair( process, timeline );
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

        void StopSession( Session& session, T_Processes processes = T_Processes() )
        {
            if( processes.first )
            {
                ExpectWebRequest( "/stop", 200 );
                MOCK_EXPECT( processes.first->Join ).returns( true );
            }
            if( processes.second )
                MOCK_EXPECT( processes.second->Kill ).once().returns( true );
            BOOST_CHECK( session.Stop() );
        }

        Session_ABC::T_Ptr ReplaySession( Session& session )
        {
            MOCK_EXPECT( uuids.Create ).once().returns( boost::uuids::random_generator()() );
            MOCK_EXPECT( nodes.LinkExerciseName ).once().with( mock::same( *node ), session.GetExercise() ).returns( FromJson( links ) );
            MOCK_EXPECT( ports.Create0 ).once().returns( new MockPort( defaultPort + 17 ) );
            return session.Replay( web::User( 42, "" ) );
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
    auto processes = StartSession( *session, processPid, processName );
    StopSession( *session, processes );
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

    auto processes = StartSession( *session, processPid, processName );

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

    StopSession( *session, processes );

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
    const auto copy = ReloadSession( save )->Save();
    BOOST_CHECK_EQUAL( ToJson( save ), ToJson( copy ) );
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_reloads, Fixture )
{
    SessionPtr session = MakeSession();
    auto processes = StartSession( *session, processPid, processName );
    const Tree save = session->Save();
    session = ReloadSession( save, processes );
    StopSession( *session, processes );
}

BOOST_FIXTURE_TEST_CASE( session_updates, Fixture )
{
    SessionPtr session = MakeSession();
    Tree save;
    save.put( "reports.clean_frequency", 38 );
    save.put( "sides.no_side_objects", false );
    Tree side1;
    side1.put( "name", "team1" );
    side1.put( "created", true );
    Tree side2;
    side2.put( "name", "team2" );
    side2.put( "created", false );
    save.put_child( "sides.list.1", side1 );
    save.put_child( "sides.list.2", side2 );
    session->Update( save );
    BOOST_CHECK_EQUAL( Get< int >( save, "reports.clean_frequency" ), 38 );
    BOOST_CHECK_EQUAL( Get< bool >( save, "sides.no_side_objects" ), false );
    BOOST_CHECK_EQUAL( Get< std::string >( save, "sides.list.1.name" ), "team1" );
    BOOST_CHECK_EQUAL( Get< bool >( save, "sides.list.2.created" ), false );
}

BOOST_FIXTURE_TEST_CASE( session_rejects_bind_to_another_process, Fixture )
{
    SessionPtr session = MakeSession();
    auto processes = StartSession( *session, processPid, processName );
    const Tree save = session->Save();
    processes.first = boost::make_shared< MockProcess >( processPid, processName + "_" );
    session = ReloadSession( save, processes, false );
    BOOST_CHECK( !session->Stop() );
}

BOOST_FIXTURE_TEST_CASE( session_can_start_twice, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    BOOST_CHECK( !session->Start( apps, apps, std::string() ) );
}

BOOST_FIXTURE_TEST_CASE( session_can_pause_and_restart, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    ExpectWebRequest( "/pause", 200 );
    BOOST_CHECK( session->Pause() );
    BOOST_CHECK( !session->Pause() );
    ExpectWebRequest( "/play", 200 );
    BOOST_CHECK( session->Start( apps, apps, std::string() ) );
}

BOOST_FIXTURE_TEST_CASE( session_discards_outdated_updates_due_to_invalidated_process, Fixture )
{
    SessionPtr session = MakeSession();
    auto processes = StartSession( *session, processPid, processName );
    Pool pool( 2, 2 );

    Event waitPause, endPause;
    ExpectBlockingWebRequest( "/pause", 200, waitPause, endPause );
    Pool_ABC::Future pause = pool.Post( [=]{ session->Pause(); } );
    waitPause.Wait();

    Event waitPoll, endPoll;
    boost::shared_ptr< MockResponse > rpy = ExpectBlockingWebRequest( "/get", 200, waitPoll, endPoll );
    MOCK_EXPECT( rpy->GetBody ).once().returns( "{\"state\":\"paused\"}" );
    Pool_ABC::Future poll = pool.Post( [=]{ session->Poll(); } );
    waitPoll.Wait();

    // stop has priority even with send states in flight
    StopSession( *session, processes );
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
    auto processes = StartSession( *session, processPid, processName );
    Pool pool( 1, 1 );

    Event waitPause, endPause;
    ExpectBlockingWebRequest( "/pause", 200, waitPause, endPause );
    Pool_ABC::Future pause = pool.Post( [=]{ session->Pause(); } );
    waitPause.Wait();

    ExpectWebRequest( "/pause", 200 );
    session->Pause();
    ExpectWebRequest( "/play", 200 );
    session->Start( apps, apps, std::string() );
    BOOST_CHECK_EQUAL( GetState( *session ), "playing" );

    endPause.Signal();
    pause.wait();
    BOOST_CHECK_EQUAL( GetState( *session ), "playing" );
}

BOOST_FIXTURE_TEST_CASE( session_can_be_removed, Fixture )
{
    SessionPtr session = MakeSession();
    auto processes = StartSession( *session, processPid, processName );
    ExpectWebRequest( "/stop", 200 );
    MOCK_EXPECT( processes.first->Join ).returns( true );
    MOCK_EXPECT( processes.second->Kill ).once().returns( true );
    MOCK_EXPECT( node->UnlinkExercise ).once();
    MOCK_EXPECT( node->RemoveSession ).once().with( defaultId );
    session->Remove();
    BOOST_CHECK_EQUAL( GetState( *session ), "stopped" );
}

BOOST_FIXTURE_TEST_CASE( session_removal_eventually_succeeds, Fixture )
{
    SessionPtr session = MakeSession();
    Pool pool( 1, 2 );

    ProcessPtr process = boost::make_shared< MockProcess >( processPid, processName );
    MOCK_EXPECT( runtime.Start ).once().returns( process );
    MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
    MOCK_EXPECT( fs.WriteFile ).once().returns( true );
    ProcessPtr timeline = boost::make_shared< MockProcess >( processPid*2, "timeline" );
    MOCK_EXPECT( runtime.Start ).once().returns( timeline );

    Event waiting, connected, started, removed;
    MOCK_EXPECT( ports.WaitConnected ).once().calls( [&] (int, std::function< bool() >) -> bool {
        waiting.Signal();
        connected.Wait();
        return true;
    } );

    bool hasStarted = false;
    auto start = pool.Post( [&] {
        hasStarted = StartSessionWith( *session, process, "" );
        started.Signal();
    } );

    BOOST_CHECK_EQUAL( GetState( *session ), "stopped" );
    waiting.Wait();
    auto remove = pool.Post( [&] {
        session->Remove();
        removed.Signal();
    } );

    waiting.Wait();
    ExpectWebRequest( "/stop", 200 );
    MOCK_EXPECT( process->Join ).returns( true );
    MOCK_EXPECT( timeline->Kill ).once().returns( true );
    MOCK_EXPECT( node->UnlinkExercise ).once();
    MOCK_EXPECT( node->RemoveSession ).once().with( defaultId );

    connected.Signal();
    started.Wait();
    BOOST_REQUIRE( hasStarted );

    removed.Wait();
    BOOST_CHECK_EQUAL( GetState( *session ), "stopped" );
}

BOOST_FIXTURE_TEST_CASE( session_cannot_replay_without_one_play, Fixture )
{
    SessionPtr session = MakeSession();
    BOOST_CHECK_THROW( session->Replay( web::User( 42, "" ) ), web::HttpException );
}

BOOST_FIXTURE_TEST_CASE( session_can_replay, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    Session_ABC::T_Ptr replay = ReplaySession( *session );
    BOOST_CHECK( replay );
}

BOOST_FIXTURE_TEST_CASE( session_cannot_restart_with_replays, Fixture )
{
    SessionPtr session = MakeSession();
    auto processes = StartSession( *session, processPid, processName );
    Session_ABC::T_Ptr replay = ReplaySession( *session );
    StopSession( *session, processes );
    BOOST_CHECK_THROW( session->Start( apps, apps, std::string() ), web::HttpException );
}

BOOST_FIXTURE_TEST_CASE( session_cannot_archive_if_not_stopped, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    BOOST_CHECK_THROW( session->Archive(), web::HttpException );
}

BOOST_FIXTURE_TEST_CASE( replay_session_cannot_be_replayed, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    Session_ABC::T_Ptr replay = ReplaySession( *session );
    BOOST_CHECK_THROW( replay->Replay( web::User( 42, "" ) ), web::HttpException );
}

namespace
{
    bool EndsWith( host::Path suffix, host::Path path )
    {
        for( ; !suffix.empty(); suffix.remove_filename(), path.remove_filename() )
            if( path.empty() )
                return false;
            else if( suffix.filename() != path.filename() )
                return false;
        return true;
    }

    void ReadCheckpoints( const host::Path& path, bool /*recurse*/,
                          const MockFileSystem& fs,
                          const FileSystem_ABC::T_Predicate& predicate )
    {
        if( !EndsWith( "checkpoints", path ) )
            return;
        MOCK_EXPECT( fs.IsFile ).with( path / "uno" / "data" ).returns( true );
        predicate( path / "uno" );
    }

    void CheckList( const Tree& tree, std::vector< std::string > items )
    {
        BOOST_REQUIRE_EQUAL( items.size(), static_cast< size_t >( std::distance( tree.begin(), tree.end() ) ) );
        for( Tree::const_iterator it = tree.begin(); it != tree.end(); ++it )
        {
            BOOST_CHECK_EQUAL( it->first, std::string() );
            BOOST_CHECK_EQUAL( Get< std::string >( it->second, std::string() ), items.front() );
            items.erase( items.begin() );
        }
    }

    void CheckCheckpoints( const Session_ABC& session )
    {
        const Tree tree = session.GetProperties();
        const std::vector< std::string > tokens = boost::assign::list_of( "checkpoints" )( "list" );
        std::vector< std::string > items = boost::assign::list_of( "uno" );
        Walk( tree, tokens.begin(), tokens.end(), boost::bind( &CheckList, _1, items ) );
    }
}

BOOST_FIXTURE_TEST_CASE( session_parses_checkpoints, Fixture )
{
    const Tree save = MakeSession()->Save();
    MOCK_RESET( fs.Walk );
    MOCK_EXPECT( fs.Walk ).calls( boost::bind( ReadCheckpoints, _1, _2, boost::ref( fs ), _3 ) );
    SessionPtr next = ReloadSession( save );
    CheckCheckpoints( *next );
    auto processes = StartSession( *next, processPid, processName, "uno" );
    StopSession( *next, processes );
    CheckCheckpoints( *next );
}

BOOST_FIXTURE_TEST_CASE( session_kills_sim_when_timeline_fails, Fixture )
{
    auto session = MakeSession();
    ProcessPtr process = boost::make_shared< MockProcess >( processPid, processName );
    // unable to open target port
    MOCK_EXPECT( runtime.Start ).once().returns( process );
    MOCK_EXPECT( ports.WaitConnected ).once().returns( false );
    MOCK_EXPECT( process->Kill ).once().returns( true );
    BOOST_REQUIRE( !StartSessionWith( *session, process ) );
    // unable to open timeline process
    MOCK_EXPECT( ports.WaitConnected ).once().returns( true );
    MOCK_EXPECT( runtime.Start ).once().returns( process );
    MOCK_EXPECT( uuids.Create ).once().returns( defaultId );
    MOCK_EXPECT( fs.WriteFile ).once().returns( true );
    MOCK_EXPECT( runtime.Start ).once().returns( ProcessPtr() );
    MOCK_EXPECT( process->Kill ).once().returns( true );
    BOOST_REQUIRE( !StartSessionWith( *session, process ) );
}
