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

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : automatId              ( 1337 )
            , automatCreationObserver( 0 )
        {
            MOCK_EXPECT( automatCreation, Register ).once().with( mock::retrieve( automatCreationObserver ) );
            MOCK_EXPECT( automatCreation, Unregister ).once();
            creationMessage.mutable_automat()->set_id( automatId );
        }
        const unsigned long automatId;
        ResponseObserver_ABC< sword::AutomatCreation >* automatCreationObserver;
        MockContextHandler< sword::AutomatCreation > automatCreation;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory contextFactory;
        sword::AutomatCreation creationMessage;
        sword::ClientToSim disengageMessage;
    };
}

BOOST_FIXTURE_TEST_CASE( automat_disengager_sends_disengage_message_when_receiving_automat_creation_message, Fixture )
{
    AutomatDisengager disengager( automatCreation, publisher, contextFactory );
    BOOST_REQUIRE( automatCreationObserver );
    MOCK_EXPECT( contextFactory, Create ).once().returns( 42 );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( disengageMessage ) );
    automatCreationObserver->Notify( creationMessage, "automat" );
    BOOST_CHECK( disengageMessage.message().has_set_automat_mode() );
    const sword::SetAutomatMode& mode = disengageMessage.message().set_automat_mode();
    BOOST_CHECK_EQUAL( mode.automate().id(), automatId );
    BOOST_CHECK_EQUAL( mode.mode(), sword::disengaged );
}
