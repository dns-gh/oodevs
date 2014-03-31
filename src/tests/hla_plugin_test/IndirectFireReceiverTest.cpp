// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/IndirectFireReceiver.h"
#include "hla_plugin/Interactions.h"
#include "MockSimulationPublisher.h"
#include "MockContextFactory.h"
#include "MockDotationTypeResolver.h"
#include "protocol/Senders.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : receiver        ( publisher, factory, munitionResolver )
            , latitude        ( 1. )
            , longitude       ( 2. )
            , munitionType    ( "1 2 3 0 0 0 0" )
            , munitionId      ( 1337 )
            , interventionType( 1. )
        {
            MOCK_EXPECT( factory.Create ).returns( 42 );
        }
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory factory;
        MockDotationTypeResolver munitionResolver;
        IndirectFireReceiver receiver;
        interactions::MunitionDetonation parameters;
        const double latitude;
        const double longitude;
        const std::string munitionType;
        const unsigned int munitionId;
        const float interventionType;
    };
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_receiver_sends_create_fire_order_on_location_when_receiving_ground_impact_munition_detonation, Fixture )
{
    sword::ClientToSim message;
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message ) );
    MOCK_EXPECT( munitionResolver.ResolveType ).once().with( rpr::EntityType( munitionType ) ).returns( munitionId );
    parameters.detonationLocation = rpr::WorldLocation( latitude, longitude, 0. );
    parameters.munitionType = rpr::EntityType( munitionType );
    receiver.Receive( parameters );
    mock::verify();
    BOOST_CHECK( message.message().has_magic_action() );
    const sword::MagicAction& action = message.message().magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::MagicAction::create_fire_order_on_location );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 3 );
    const sword::Location& location = action.parameters().elem( 0 ).value( 0 ).location();
    BOOST_CHECK_EQUAL( location.type(), sword::Location::point );
    BOOST_CHECK_EQUAL( location.coordinates().elem_size(), 1 );
    BOOST_CHECK_CLOSE( location.coordinates().elem( 0 ).latitude(), latitude, 0.0001 );
    BOOST_CHECK_CLOSE( location.coordinates().elem( 0 ).longitude(), longitude, 0.0001 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).resourcetype().id(), munitionId );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).areal(), interventionType );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_receiver_does_nothing_if_target_is_defined, Fixture )
{
    parameters.targetObjectIdentifier = Omt13String( "local target" );
    receiver.Receive( parameters );
}
