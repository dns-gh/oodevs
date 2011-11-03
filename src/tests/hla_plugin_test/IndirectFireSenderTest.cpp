// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/IndirectFireSender.h"
#include "hla_plugin/Omt13String.h"
#include "hla_plugin/Interactions.h"
#include "MockMessageController.h"
#include "MockInteractionSender.h"
#include "MockMunitionTypeResolver.h"
#include "MockLocalAgentResolver.h"
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
            : sender              ( interactionSender, controller, "federate", munitionTypeResolver, localResolver )
            , fireIdentifier      ( 42 )
            , firingUnitIdentifier( 1338 )
        {
            startMessage.mutable_start_unit_fire()->mutable_firing_unit()->set_id( firingUnitIdentifier );
            stopMessage.mutable_stop_unit_fire();
        }
        tools::MessageController< sword::SimToClient_Content > controller;
        MockInteractionSender< interactions::MunitionDetonation > interactionSender;
        MockLocalAgentResolver localResolver;
        MockMunitionTypeResolver munitionTypeResolver;
        IndirectFireSender sender;
        sword::SimToClient_Content startMessage;
        sword::SimToClient_Content stopMessage;
        unsigned int fireIdentifier;
        unsigned int firingUnitIdentifier;
    };
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_does_nothing_when_no_start_unit_fire, Fixture )
{
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_does_nothing_when_no_start_unit_fire_id_matches_with_stop_message, Fixture )
{
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    controller.Dispatch( startMessage );
    stopMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier + 1 );
    controller.Dispatch( stopMessage );
}

namespace
{
    class ConfiguredFixture : public Fixture
    {
    public:
        ConfiguredFixture()
            : latitude      ( 1. )
            , longitude     ( 2. )
            , ammunitionType( 123 )
            , ammunitionName( "1 2 3 0 0 0 0" )
        {
            startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
            startMessage.mutable_start_unit_fire()->mutable_ammunition()->set_id( ammunitionType );
            sword::CoordLatLong* position = startMessage.mutable_start_unit_fire()->mutable_target()->mutable_position();
            position->set_latitude( latitude );
            position->set_longitude( longitude );
            startMessage.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( 1338 );
            stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            MOCK_EXPECT( localResolver, ResolveIdentifier ).once().with( firingUnitIdentifier ).returns( "local" );
            controller.Dispatch( startMessage );
            MOCK_EXPECT( interactionSender, Send ).once().with( mock::retrieve( parameters ) );
            MOCK_EXPECT( munitionTypeResolver, ResolveIdentifier ).once().with( ammunitionType ).returns( rpr::EntityType( ammunitionName ) );
            controller.Dispatch( stopMessage );
        }
        interactions::MunitionDetonation parameters;
        const double latitude;
        const double longitude;
        const unsigned int ammunitionType;
        const std::string ammunitionName;
    };
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_does_not_resend_an_already_sent_message, ConfiguredFixture )
{
    mock::verify();
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_articulated_part_data_is_always_empty, ConfiguredFixture )
{
    BOOST_CHECK( parameters.articulatedPartData.empty() );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_sends_position_location, ConfiguredFixture )
{
    BOOST_CHECK_CLOSE( parameters.detonationLocation.Latitude(),  latitude,  0.00001 );
    BOOST_CHECK_CLOSE( parameters.detonationLocation.Longitude(), longitude, 0.00001 );
    BOOST_CHECK_SMALL( parameters.detonationLocation.Altitude()            , 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_sends_ground_impact, ConfiguredFixture )
{
    const int8 groundImpact = 3;
    BOOST_CHECK_EQUAL( parameters.detonationResultCode, groundImpact );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_uses_fire_identifier_for_event_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.eventIdentifier.eventCount, fireIdentifier );
    BOOST_CHECK_EQUAL( parameters.eventIdentifier.issuingObjectIdentifier.str(), "federate" );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_uses_simulation_identifier_for_firing_object_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.firingObjectIdentifier.str(), "local" );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_sends_constant_final_velocity_vector, ConfiguredFixture )
{
    BOOST_CHECK_SMALL( parameters.finalVelocityVector.VX(),       0.00001 );
    BOOST_CHECK_SMALL( parameters.finalVelocityVector.VY(),       0.00001 );
    BOOST_CHECK_CLOSE( parameters.finalVelocityVector.VZ(), 400., 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_sends_other_fuse_type, ConfiguredFixture )
{
    const int16 other = 0;
    BOOST_CHECK_EQUAL( parameters.fuseType, other );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_empty_munition_object_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.munitionObjectIdentifier.str(), "" );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_resolved_munition_type, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.munitionType.str(), ammunitionName );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_constant_quantity_fired, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.quantityFired, 3 );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_one_rate_of_fire, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.rateOfFire, 3 );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_empty_relative_detonation_location, ConfiguredFixture )
{
    BOOST_CHECK_SMALL( parameters.relativeDetonationLocation.X(), 0.00001 );
    BOOST_CHECK_SMALL( parameters.relativeDetonationLocation.Y(), 0.00001 );
    BOOST_CHECK_SMALL( parameters.relativeDetonationLocation.Z(), 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_empty_target_object_identifier, ConfiguredFixture )
{
    BOOST_CHECK_EQUAL( parameters.targetObjectIdentifier.str(), "" );
}

BOOST_FIXTURE_TEST_CASE( indirect_fire_sender_send_constant_warhead_type_to_other, ConfiguredFixture )
{
    const int16 other = 0;
    BOOST_CHECK_EQUAL( parameters.warheadType, other );
}
