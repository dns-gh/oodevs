// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/DirectFireReceiver.h"
#include "hla_plugin/Interactions.h"
#include "MockRemoteAgentResolver.h"
#include "MockSimulationPublisher.h"
#include "MockContextFactory.h"
#include "protocol/Senders.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : receiver        ( publisher, resolver, factory )
            , entityImpact    ( 1 )
            , sourceIdentifier( 42 )
            , targetIdentifier( 43 )
        {
            MOCK_EXPECT( factory, Create ).returns( 42 );
        }
        MockRemoteAgentResolver resolver;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory factory;
        DirectFireReceiver receiver;
        interactions::MunitionDetonation parameters;
        const int8 entityImpact;
        const unsigned int sourceIdentifier;
        const unsigned int targetIdentifier;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_receiver_sends_create_direct_fire_order_when_receiving_entity_impact_munition_detonation, Fixture )
{
    sword::ClientToSim message;
    MOCK_EXPECT( resolver, ResolveName ).once().with( "source" ).returns( sourceIdentifier );
    MOCK_EXPECT( resolver, ResolveName ).once().with( "target" ).returns( targetIdentifier );
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    parameters.detonationResultCode = entityImpact;
    parameters.firingObjectIdentifier = Omt13String( "source" );
    parameters.targetObjectIdentifier = Omt13String( "target" );
    receiver.Receive( parameters );
    mock::verify();
    BOOST_CHECK( message.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = message.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::create_direct_fire_order );
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), sourceIdentifier );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    BOOST_CHECK_EQUAL( targetIdentifier, action.parameters().elem( 0 ).value( 0 ).agent().id() );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_receiver_does_nothing_when_receiving_not_entity_impact_munition_detonation, Fixture )
{
    parameters.detonationResultCode = 0;
    receiver.Receive( parameters );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_receiver_does_nothing_when_receiving_mal_formed_entity_impact_munition_detonation, Fixture )
{
    parameters.detonationResultCode = entityImpact;
    receiver.Receive( parameters );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_receiver_does_nothing_when_receiving_mal_formed_entity_impact_munition_detonation_2, Fixture )
{
    parameters.detonationResultCode = entityImpact;
    parameters.firingObjectIdentifier = Omt13String( "unknown" );
    parameters.targetObjectIdentifier = Omt13String( "target" );
    MOCK_EXPECT( resolver, ResolveName ).once().with( "unknown" ).returns( 0 );
    receiver.Receive( parameters );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_receiver_does_nothing_when_receiving_mal_formed_entity_impact_munition_detonation_3, Fixture )
{
    parameters.detonationResultCode = entityImpact;
    parameters.firingObjectIdentifier = Omt13String( "source" );
    parameters.targetObjectIdentifier = Omt13String( "unknown" );
    MOCK_EXPECT( resolver, ResolveName ).once().with( "source" ).returns( sourceIdentifier );
    MOCK_EXPECT( resolver, ResolveName ).once().with( "unknown" ).returns( 0 );
    receiver.Receive( parameters );
}