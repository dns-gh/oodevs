// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/UnitTeleporter.h"
#include "MockAgentSubject.h"
#include "MockSimulationPublisher.h"
#include "MockContextHandler.h"
#include "MockContextFactory.h"
#include "MockRemoteAgentSubject.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : unitId( 42 )
            , latitude( 1. )
            , longitude( 2. )
            , remoteAgentListener( 0 )
            , unitCreationObserver( 0 )
        {
            MOCK_EXPECT( agentSubject, Register ).once().with( mock::retrieve( remoteAgentListener ) );
            MOCK_EXPECT( contextHandler, Register ).once().with( mock::retrieve( unitCreationObserver ) );
            MOCK_EXPECT( agentSubject, Unregister ).once();
            MOCK_EXPECT( contextHandler, Unregister ).once();
            creationMessage.mutable_unit()->set_id( unitId );
        }
        const unsigned long unitId;
        const double latitude;
        const double longitude;
        MockRemoteAgentSubject agentSubject;
        MockContextHandler< sword::UnitCreation > contextHandler;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory contextFactory;
        RemoteAgentListener_ABC* remoteAgentListener;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationObserver;
        sword::UnitCreation creationMessage;
        sword::ClientToSim teleportMessage;
    };
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_teleports_unit, Fixture )
{
    UnitTeleporter teleporter( agentSubject, contextHandler, publisher, contextFactory );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    unitCreationObserver->Notify( creationMessage, "identifier" );
    MOCK_EXPECT( contextFactory, Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( teleportMessage ) );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    BOOST_CHECK( teleportMessage.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = teleportMessage.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::move_to );
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), unitId );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    const sword::Location& location = action.parameters().elem( 0 ).value( 0 ).point().location();
    BOOST_CHECK_EQUAL( location.type(), sword::Location::point );
    BOOST_CHECK_EQUAL( location.coordinates().elem_size(), 1 );
    BOOST_CHECK_EQUAL( location.coordinates().elem( 0 ).latitude(), latitude );
    BOOST_CHECK_EQUAL( location.coordinates().elem( 0 ).longitude(), longitude );
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_teleports_only_when_unit_has_been_created, Fixture )
{
    UnitTeleporter teleporter( agentSubject, contextHandler, publisher, contextFactory );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    remoteAgentListener->Moved( "identifier", 1., 2. );
    unitCreationObserver->Notify( creationMessage, "identifier" );
    MOCK_EXPECT( contextFactory, Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher, SendClientToSim ).once();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
}
