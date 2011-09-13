// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/AutomatDisengager.h"
#include "protocol/Simulation.h"
#include "MockContextHandler.h"
#include "MockContextFactory.h"
#include "MockSimulationPublisher.h"

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( automat_disengager_sends_disengage_message_when_receiving_automat_creation_message )
{
    const unsigned long automatId = 1337;
    ResponseObserver_ABC< sword::AutomatCreation >* automatCreationObserver = 0;
    MockContextHandler< sword::AutomatCreation > automatCreation;
    dispatcher::MockSimulationPublisher publisher;
    MockContextFactory contextFactory;
    MOCK_EXPECT( automatCreation, Register ).once().with( mock::retrieve( automatCreationObserver ) );
    AutomatDisengager disengager( automatCreation, publisher, contextFactory );
    mock::verify();
    BOOST_REQUIRE( automatCreationObserver );
    sword::AutomatCreation creationMessage;
    creationMessage.mutable_automat()->set_id( automatId );
    sword::ClientToSim disengageMessage;
    MOCK_EXPECT( contextFactory, Create ).once().returns( 42 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( disengageMessage ) );
    automatCreationObserver->Notify( creationMessage, "automat" );
    mock::verify();
    BOOST_CHECK( disengageMessage.message().has_set_automat_mode() );
    const sword::SetAutomatMode& mode = disengageMessage.message().set_automat_mode();
    BOOST_CHECK_EQUAL( mode.automate().id(), automatId );
    BOOST_CHECK_EQUAL( mode.mode(), sword::disengaged );
    MOCK_EXPECT( automatCreation, Unregister ).once();
}
