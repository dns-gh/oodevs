// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/MunitionDetonation.h"
#include "hla_plugin/Omt13String.h"
#include "DeserializationTools.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockMessageController.h"
#include "MockRemoteAgentSubject.h"
#include "MockRemoteAgentResolver.h"
#include "rpr/Coordinates.h"
#include "protocol/Simulation.h"
#include "tools/MessageController.h"
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( munition_detonation_registration_publish_only )
{
    MockRemoteAgentSubject remoteAgentSubject;
    MOCK_EXPECT( remoteAgentSubject, Register ).once();
    MOCK_EXPECT( remoteAgentSubject, Unregister ).once();
    MockRemoteAgentResolver remoteAgentResolver;
    MockFederate federate;
    hla::MockInteractionHandler* handler = new hla::MockInteractionHandler(); // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
    MOCK_EXPECT( federate, RegisterInteraction ).once().with( "MunitionDetonation", mock::any, true, false ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );
    tools::MockMessageController< sword::SimToClient_Content > controller;
    MOCK_EXPECT( controller, Register ).exactly( 2 );
    MunitionDetonation interaction( federate, controller, remoteAgentResolver, remoteAgentSubject, "federate" );
    MOCK_EXPECT( controller, Unregister ).exactly( 2 );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : handler            ( new hla::MockInteractionHandler() ) // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
            , remoteAgentListener( 0 )
        {
            MOCK_EXPECT( remoteAgentSubject, Register ).once().with( mock::retrieve( remoteAgentListener ) );
            MOCK_EXPECT( remoteAgentSubject, Unregister );
            MOCK_EXPECT( federate, RegisterInteraction ).once().with( "MunitionDetonation", mock::any, true, false ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );;
        }
        MockFederate federate;
        hla::MockInteractionHandler* handler;
        MockRemoteAgentResolver remoteAgentResolver;
        tools::MessageController< sword::SimToClient_Content > controller;
        MockRemoteAgentSubject remoteAgentSubject;
        RemoteAgentListener_ABC* remoteAgentListener;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        typedef std::vector< int8 > T_Buffer;

        RegisteredFixture()
            : interaction         ( federate, controller, remoteAgentResolver, remoteAgentSubject, "federate" )
            , deserializer        ( 0 )
            , fireIdentifier      ( 42 )
            , firingUnitIdentifier( 1338 )
        {
            startMessage.mutable_start_unit_fire()->mutable_firing_unit()->set_id( firingUnitIdentifier );
            stopMessage.mutable_stop_unit_fire();
        }
        void Deserialize( const ::hla::Serializer& serializer )
        {
            buffer.resize( serializer.GetSize() );
            if( !buffer.empty() )
            {
                serializer.CopyTo( &buffer[0] );
                deserializer = ::hla::Deserializer( &buffer[0], buffer.size() );
            }
        }
        void ConfigureExpectations( const std::string& parameter )
        {
            MOCK_EXPECT( handler, Visit ).once().with( parameter, mock::any ).calls( boost::bind( &RegisteredFixture::Deserialize, this, _2 ) );
            MOCK_EXPECT( handler, Visit );
            MOCK_EXPECT( handler, End );
        }
        MunitionDetonation interaction;
        mock::sequence s;
        sword::SimToClient_Content startMessage;
        sword::SimToClient_Content stopMessage;
        T_Buffer buffer;
        ::hla::Deserializer deserializer;
        unsigned int fireIdentifier;
        unsigned int firingUnitIdentifier;
    };
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_is_sent_when_receiving_start_and_stop_unit_fire_messages, RegisteredFixture )
{
    const std::vector< std::string > parameters = boost::assign::list_of( "ArticulatedPartData" )
                                                                        ( "DetonationLocation" )
                                                                        ( "DetonationResultCode" )
                                                                        ( "EventIdentifier" )
                                                                        ( "FiringObjectIdentifier" )
                                                                        ( "FinalVelocityVector" )
                                                                        ( "FuseType" )
                                                                        ( "MunitionObjectIdentifier" )
                                                                        ( "MunitionType" )
                                                                        ( "QuantityFired" )
                                                                        ( "RateOfFire" )
                                                                        ( "RelativeDetonationLocation" )
                                                                        ( "TargetObjectIdentifier" )
                                                                        ( "WarheadType" );
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    controller.Dispatch( startMessage );
    BOOST_FOREACH( const std::string& parameter, parameters )
        MOCK_EXPECT( handler, Visit ).once().in( s ).with( parameter, mock::any );
    MOCK_EXPECT( handler, End ).once().in( s );
    MOCK_EXPECT( remoteAgentResolver, Resolve ).returns( "distant" );
    stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_does_nothing_when_no_start_unit_fire, RegisteredFixture )
{
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_does_nothing_when_no_start_unit_fire_id_matches_with_stop_message, RegisteredFixture )
{
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    controller.Dispatch( startMessage );
    stopMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier + 1 );
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_does_not_resend_an_already_sent_message, RegisteredFixture )
{
    startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    controller.Dispatch( startMessage );
    MOCK_EXPECT( remoteAgentResolver, Resolve ).returns( "distant" );
    MOCK_EXPECT( handler, Visit ).exactly( 14 );
    MOCK_EXPECT( handler, End ).once();
    stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
    controller.Dispatch( stopMessage );
    mock::verify();
    controller.Dispatch( stopMessage );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_sends_interaction_only_if_target_is_distant, RegisteredFixture )
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
        {
            startMessage.mutable_start_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            stopMessage.mutable_stop_unit_fire()->mutable_fire()->set_id( fireIdentifier );
            MOCK_EXPECT( remoteAgentResolver, Resolve ).returns( "distant" );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( articulated_part_data_is_always_empty, ConfiguredFixture )
{
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    sword::CoordLatLong* position = startMessage.mutable_start_unit_fire()->mutable_target()->mutable_position();
    position->set_latitude( 1. );
    position->set_longitude( 2. );
    ConfigureExpectations( "ArticulatedPartData" );
    controller.Dispatch( startMessage );
    controller.Dispatch( stopMessage );
    const uint32 arraySize = 0u;
    BOOST_CHECK_EQUAL( arraySize, Read< uint32 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_sends_target_location_if_direct_fire, ConfiguredFixture )
{
    BOOST_REQUIRE( remoteAgentListener );
    remoteAgentListener->Moved( "distant", 1., 2. );
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::direct );
    startMessage.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( 1337 );
    controller.Dispatch( startMessage );
    ConfigureExpectations( "DetonationLocation" );
    controller.Dispatch( stopMessage );
    rpr::WorldLocation location;
    location.Deserialize( deserializer );
    BOOST_CHECK_CLOSE( location.Latitude(),  1., 0.00001 );
    BOOST_CHECK_CLOSE( location.Longitude(), 2., 0.00001 );
    BOOST_CHECK_SMALL( location.Altitude()     , 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_sends_explosion_location_if_indirect_fire, ConfiguredFixture )
{
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    sword::CoordLatLong* position = startMessage.mutable_start_unit_fire()->mutable_target()->mutable_position();
    position->set_latitude( 1. );
    position->set_longitude( 2. );
    controller.Dispatch( startMessage );
    ConfigureExpectations( "DetonationLocation" );
    controller.Dispatch( stopMessage );
    rpr::WorldLocation location;
    location.Deserialize( deserializer );
    BOOST_CHECK_CLOSE( location.Latitude(),  1., 0.00001 );
    BOOST_CHECK_CLOSE( location.Longitude(), 2., 0.00001 );
    BOOST_CHECK_SMALL( location.Altitude()     , 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_sends_entity_impact_when_receiving_direct_fire_message, ConfiguredFixture )
{
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::direct );
    startMessage.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( 1337 );
    controller.Dispatch( startMessage );
    ConfigureExpectations( "DetonationResultCode" );
    controller.Dispatch( stopMessage );
    const int8 entityImpact = 1;
    BOOST_CHECK_EQUAL( entityImpact, Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_sends_ground_impact_when_receiving_indirect_fire_message, ConfiguredFixture )
{
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    sword::CoordLatLong* position = startMessage.mutable_start_unit_fire()->mutable_target()->mutable_position();
    position->set_latitude( 1. );
    position->set_longitude( 2. );
    controller.Dispatch( startMessage );
    ConfigureExpectations( "DetonationResultCode" );
    controller.Dispatch( stopMessage );
    const int8 groundImpact = 3;
    BOOST_CHECK_EQUAL( groundImpact, Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_uses_fire_identifier_for_event_identifier, ConfiguredFixture )
{
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    controller.Dispatch( startMessage );
    ConfigureExpectations( "EventIdentifier" );
    controller.Dispatch( stopMessage );
    BOOST_CHECK_EQUAL( fireIdentifier, Read< uint16 >( deserializer ) );
    Omt13String issuingObject;
    issuingObject.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( "federate", issuingObject.str() );
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_uses_simulation_identifier_for_firing_object_identifier, ConfiguredFixture )
{
    startMessage.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    controller.Dispatch( startMessage );
    ConfigureExpectations( "FiringObjectIdentifier" );
    controller.Dispatch( stopMessage );
    Omt13String issuingObject;
    issuingObject.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( boost::lexical_cast< std::string >( firingUnitIdentifier ), issuingObject.str() );
}
