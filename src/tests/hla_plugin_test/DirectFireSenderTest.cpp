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
        tools::MessageController< sword::SimToClient_Content > controller;
        MockRemoteAgentSubject remoteAgentSubject;
        RemoteAgentListener_ABC* remoteAgentListener;
        MockInteractionSender< interactions::MunitionDetonation > interactionSender;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : sender              ( interactionSender, remoteAgentResolver, remoteAgentSubject, controller, "federate" )
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
    MOCK_EXPECT( remoteAgentResolver, Resolve ).returns( "distant" );
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
    MOCK_EXPECT( remoteAgentResolver, Resolve ).once().with( 1337u ).returns( "" );
    controller.Dispatch( stopMessage );
}

namespace
{
    class ConfiguredFixture : public RegisteredFixture
    {
    public:
        ConfiguredFixture()
            : latitude ( 1. )
            , longitude( 2. )
        {
            BOOST_REQUIRE( remoteAgentListener );
            remoteAgentListener->Moved( "distant", latitude, longitude );
            startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::direct );
            startMessage.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( 1338 );
            stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            MOCK_EXPECT( remoteAgentResolver, Resolve ).returns( "distant" );
            controller.Dispatch( startMessage );
            MOCK_EXPECT( interactionSender, Send ).once().with( mock::retrieve( parameters ) );
            controller.Dispatch( stopMessage );
        }
        interactions::MunitionDetonation parameters;
        const double latitude;
        const double longitude;
    };
}

BOOST_FIXTURE_TEST_CASE( direct_fire_sender_articulated_part_data_is_always_empty, ConfiguredFixture )
{
    const unsigned int size = 0u;
    BOOST_CHECK_EQUAL( size, parameters.articulatedPartData );
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
    BOOST_CHECK_EQUAL( parameters.firingObjectIdentifier.str(), boost::lexical_cast< std::string >( firingUnitIdentifier ) );
}
