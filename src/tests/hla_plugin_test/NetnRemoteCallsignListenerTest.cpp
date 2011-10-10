// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnRemoteCallsignListener.h"
#include "MockRemoteAgentSubject.h"
#include "MockCallsignResolver.h"
#include "MockContextHandler.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : listener        ( 0 )
            , responseObserver( 0 )
        {
            MOCK_EXPECT( subject, Register ).once().with( mock::retrieve( listener ) );
            MOCK_EXPECT( subject, Unregister ).once();
            MOCK_EXPECT( handler, Register ).once().with( mock::retrieve( responseObserver ) );
            MOCK_EXPECT( handler, Unregister ).once();
        }
        MockCallsignResolver resolver;
        MockRemoteAgentSubject subject;
        MockContextHandler< sword::UnitCreation > handler;
        RemoteAgentListener_ABC* listener;
        ResponseObserver_ABC< sword::UnitCreation >* responseObserver;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : callsignListener( resolver, subject, handler )
        {
            BOOST_REQUIRE( listener );
            BOOST_REQUIRE( responseObserver );
        }
        sword::UnitCreation MakeCreation( unsigned int identifier )
        {
            sword::UnitCreation message;
            message.mutable_unit()->set_id( identifier );
            return message;
        }
        NetnRemoteCallsignListener callsignListener;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_remote_callsign_listener_adds_callsign_when_unit_creation_received, RegisteredFixture )
{
    listener->CallsignChanged( "identifier", "callsign" );
    listener->UniqueIdChanged( "identifier", "unique" );
    MOCK_EXPECT( resolver, Add ).once().with( 42u, "callsign", "unique" );
    responseObserver->Notify( MakeCreation( 42u ), "identifier" );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_callsign_listener_adds_callsign_when_callsign_received, RegisteredFixture )
{
    listener->UniqueIdChanged( "identifier", "unique" );
    responseObserver->Notify( MakeCreation( 42u ), "identifier" );
    MOCK_EXPECT( resolver, Add ).once().with( 42u, "callsign", "unique" );
    listener->CallsignChanged( "identifier", "callsign" );
}

BOOST_FIXTURE_TEST_CASE( netn_remote_callsign_listener_adds_callsign_when_uniqueid_received, RegisteredFixture )
{
    listener->CallsignChanged( "identifier", "callsign" );
    responseObserver->Notify( MakeCreation( 42u ), "identifier" );
    MOCK_EXPECT( resolver, Add ).once().with( 42u, "callsign", "unique" );
    listener->UniqueIdChanged( "identifier", "unique" );
}
