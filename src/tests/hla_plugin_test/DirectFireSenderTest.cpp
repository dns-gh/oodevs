// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/DirectFireSender.h"
#include "hla_plugin/Omt13String.h"
#include "hla_plugin/Interactions.h"
#include "MockMessageController.h"
#include "MockLocalAgentResolver.h"
#include "MockRemoteAgentSubject.h"
#include "MockRemoteAgentResolver.h"
#include "MockInteractionSender.h"
#include "rpr/Coordinates.h"
#include "protocol/Simulation.h"
#include "tools/MessageController.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : remoteAgentListener( 0 )
        {
            MOCK_EXPECT( remoteAgentSubject, Register ).once().with( mock::retrieve( remoteAgentListener ) );
            MOCK_EXPECT( remoteAgentSubject, Unregister );
        }
        MockRemoteAgentResolver remoteAgentResolver;
        MockLocalAgentResolver localAgentResolver;
        tools::MessageController< sword::SimToClient_Content > controller;
        MockRemoteAgentSubject remoteAgentSubject;
        RemoteAgentListener_ABC* remoteAgentListener;
        MockInteractionSender< interactions::MunitionDetonation > interactionSender;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : sender              ( interactionSender, remoteAgentResolver, localAgentResolver, remoteAgentSubject, controller, "federate" )
            , fireIdentifier      ( 42 )
            , firingUnitIdentifier( 1338 )
        {
            startMessage.mutable_start_unit_fire()->mutable_firing_unit()->set_id( firingUnitIdentifier );
            stopMessage.mutable_stop_unit_fire();
        }
        DirectFireSender sender;
        sword::SimToClient_Content startMessage;
        sword::SimToClient_Content stopMessage;
        unsigned int fireIdentifier;
        unsigned int firingUnitIdentifier;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_does_nothing_when_no_start_unit_fire, RegisteredFixture )
{
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_does_nothing_when_no_start_unit_fire_id_matches_with_stop_message, RegisteredFixture )
{
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    controller.Dispatch( startMessage );
    stopMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier + 1 );
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_does_not_resend_an_already_sent_message, RegisteredFixture )
{
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::direct );
    controller.Dispatch( startMessage );
    MOCK_EXPECT( remoteAgentResolver, ResolveIdentifier ).returns( "distant" );
    MOCK_EXPECT( localAgentResolver, ResolveIdentifier ).returns( "local" );
    MOCK_EXPECT( interactionSender, Send ).once();
    stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    controller.Dispatch( stopMessage );
    mock::verify();
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_does_not_send_if_target_is_not_distant, RegisteredFixture )
{
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::direct );
    startMessage.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( 1337 );
    controller.Dispatch( startMessage );
    MOCK_EXPECT( remoteAgentResolver, ResolveIdentifier ).once().with( 1337u ).returns( "" );
    controller.Dispatch( stopMessage );
}

namespace
{
    class ConfiguredFixture : public RegisteredFixture
    {
    public:
        ConfiguredFixture()
            : latitude        ( 1. )
            , longitude       ( 2. )
            , targetIdentifier( 1337 )
        {
            BOOST_REQUIRE( remoteAgentListener );
            remoteAgentListener->Moved( "distant", latitude, longitude );
            startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::direct );
            startMessage.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( targetIdentifier );
            stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            MOCK_EXPECT( remoteAgentResolver, ResolveIdentifier ).once().with( targetIdentifier ).returns( "distant" );
            MOCK_EXPECT( localAgentResolver, ResolveIdentifier ).once().with( firingUnitIdentifier ).returns( "local" );
            controller.Dispatch( startMessage );
            MOCK_EXPECT( interactionSender, Send ).once().with( mock::retrieve( parameters ) );
            controller.Dispatch( stopMessage );
        }
        interactions::MunitionDetonation parameters;
        const unsigned int targetIdentifier;
        const double latitude;
        const double longitude;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_articulated_part_data_is_always_empty, ConfiguredFixture )
{
    BOOST_CHECK( parameters.articulatedPartData.empty() );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_sends_target_location, ConfiguredFixture )
{
    BOOST_CHECK_CLOSE( parameters.detonationLocation.Latitude(),  latitude,  0.00001 );
    BOOST_CHECK_CLOSE( parameters.detonationLocation.Longitude(), longitude, 0.00001 );
    BOOST_CHECK_SMALL( parameters.detonationLocation.Altitude()            , 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_sends_entity_impact, ConfiguredFixture )
{
    const int8 entityImpact = 1;
    BOOST_CHECK_EQUAL( parameters.detonationResultCode, entityImpact );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_uses_fire_identifier_for_event_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.eventIdentifier.eventCount, fireIdentifier );
    BOOST_CHECK_EQUAL( parameters.eventIdentifier.issuingObjectIdentifier.str(), "federate" );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_uses_simulation_identifier_for_firing_object_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.firingObjectIdentifier.str(), "local" );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_sends_constant_final_velocity_vector, ConfiguredFixture )
{
    BOOST_CHECK_SMALL( parameters.finalVelocityVector.VX(),       0.00001 );
    BOOST_CHECK_SMALL( parameters.finalVelocityVector.VY(),       0.00001 );
    BOOST_CHECK_CLOSE( parameters.finalVelocityVector.VZ(), 700., 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_sends_other_fuse_type, ConfiguredFixture )
{
    const int16 other = 0;
    BOOST_CHECK_EQUAL( parameters.fuseType, other );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_empty_munition_object_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.munitionObjectIdentifier.str(), "" );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_constant_7_62mm_munition_type, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.munitionType.str(), "2 8 71 2 10 0 0" );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_constant_7_62mm_quantity_fired, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.quantityFired, 10 );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_constant_7_62mm_rate_of_fire, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.rateOfFire, 40 );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_empty_relative_detonation_location, ConfiguredFixture )
{
    BOOST_CHECK_SMALL( parameters.relativeDetonationLocation.X(), 0.00001 );
    BOOST_CHECK_SMALL( parameters.relativeDetonationLocation.Y(), 0.00001 );
    BOOST_CHECK_SMALL( parameters.relativeDetonationLocation.Z(), 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_target_object_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.targetObjectIdentifier.str(), "distant" );
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_send_constant_warhead_type_to_other, ConfiguredFixture )
{
    const int16 other = 0;
    BOOST_CHECK_EQUAL( parameters.warheadType, other );
}
