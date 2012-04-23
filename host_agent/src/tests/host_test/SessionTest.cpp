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
#include <host/Session.h>
#include <host/Json.h>

#include <boost/make_shared.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid//uuid_io.hpp>

using namespace host;
using mocks::MockPort;
using mocks::MockPortFactory;
using mocks::MockProcess;
using mocks::MockRuntime;

namespace
{
    const std::string defaultIdString = "12345678-90AB-CDEF-9876-543210123456";
    const boost::uuids::uuid defaultId = boost::uuids::string_generator()( defaultIdString );
    const std::string defaultNodeString = "10123456-CDEF-9876-90AB-543212345678";
    const boost::uuids::uuid defaultNode = boost::uuids::string_generator()( defaultNodeString );
    const std::string defaultName = "myName";
    const std::string defaultExercise = "MyExercise";
    const int defaultPort = 1337;
    const int processPid = 7331;
    const std::string processName = "myProcessName";

    struct Fixture
    {
        typedef boost::shared_ptr< Session > SessionPtr;
        typedef boost::shared_ptr< MockProcess > ProcessPtr;
        MockRuntime runtime;
        MockPortFactory ports;
        MOCK_FUNCTOR( Starter, ProcessPtr( const Session& ) );
        Fixture()
        {
            // NOTHING
        }

        SessionPtr MakeSession()
        {
            return boost::make_shared< Session >( defaultId, defaultNode, defaultName, defaultExercise, std::auto_ptr< Port_ABC >( new MockPort( defaultPort ) ) );
        }

        SessionPtr ReloadSession( const boost::property_tree::ptree& tree, ProcessPtr process = ProcessPtr() )
        {
            MOCK_EXPECT( ports.Create1 ).once().with( defaultPort ).returns( new MockPort( defaultPort ) );
            if( process )
                MOCK_EXPECT( runtime.GetProcess ).once().with( process->GetPid() ).returns( process );
            return boost::make_shared< Session >( tree, runtime, ports );
        }

        ProcessPtr StartSession( Session& session, int pid, const std::string& name )
        {
            ProcessPtr process = boost::make_shared< MockProcess >( pid, name );
            MOCK_EXPECT( Starter ).with( mock::same( session ) ).returns( process );
            BOOST_REQUIRE( session.Start( Starter ) );
            return process;
        }

        void StopSession( Session& session, ProcessPtr process = ProcessPtr() )
        {
            if( process )
                MOCK_EXPECT( process->Kill ).once().returns( true );
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
}

BOOST_FIXTURE_TEST_CASE( session_converts, Fixture )
{
    SessionPtr session = MakeSession();
    const std::string base = "{"
        "\"id\":\"12345678-90ab-cdef-9876-543210123456\","
        "\"node\":\"10123456-cdef-9876-90ab-543212345678\","
        "\"name\":\"myName\","
        "\"exercise\":\"MyExercise\","
        "\"port\":\"1337\",";
    BOOST_CHECK_EQUAL( ToJson( session->GetProperties() ), base +
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( session->Save() ), base +
        "\"status\":\"stopped\""
        "}" );
    ProcessPtr process = StartSession( *session, processPid, processName );
    BOOST_CHECK_EQUAL( ToJson( session->GetProperties() ), base +
        "\"status\":\"playing\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( session->Save() ), base +
        "\"status\":\"playing\","
        "\"process\":{"
            "\"pid\":\"7331\","
            "\"name\":\"myProcessName\""
        "}}" );
    StopSession( *session, process );
    BOOST_CHECK_EQUAL( ToJson( session->GetProperties() ), base +
        "\"status\":\"stopped\""
        "}" );
    BOOST_CHECK_EQUAL( ToJson( session->Save() ), base +
        "\"status\":\"stopped\""
        "}" );
}

BOOST_FIXTURE_TEST_CASE( session_reloads, Fixture )
{
    const boost::property_tree::ptree save = MakeSession()->Save();
    ReloadSession( save );
}

BOOST_FIXTURE_TEST_CASE( session_starts_and_reloads, Fixture )
{
    SessionPtr session = MakeSession();
    ProcessPtr process = StartSession( *session, processPid, processName );
    const boost::property_tree::ptree save = session->Save();
    session = ReloadSession( save, process );
    StopSession( *session, process );
}

BOOST_FIXTURE_TEST_CASE( session_rejects_bind_to_another_process, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    const boost::property_tree::ptree save = session->Save();
    session = ReloadSession( save, boost::make_shared< MockProcess >( processPid, processName + "_" ) );
    BOOST_CHECK( session->Stop() );
}

BOOST_FIXTURE_TEST_CASE( session_can_start_twice, Fixture )
{
    SessionPtr session = MakeSession();
    StartSession( *session, processPid, processName );
    BOOST_CHECK( session->Start( Starter ) );
}
