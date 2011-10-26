// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/TransportationOfferer.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/TransportedUnitsVisitor_ABC.h"
#include "MockInteractionSender.h"
#include "MockContextFactory.h"
#include "MockTransportedUnits.h"
#include "MockTransporters.h"
#include "MockCallsignResolver.h"
#include "tools/MessageController.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : offerConvoySender( offerInteractionSender, serviceStartedInteractionSender,
                                 convoyEmbarkmentStatusInteractionSender, transporters, messageController, callsignResolver )
        {}
        MockInteractionSender< interactions::NetnOfferConvoy > offerInteractionSender;
        MockInteractionSender< interactions::NetnServiceStarted > serviceStartedInteractionSender;
        MockInteractionSender< interactions::NetnConvoyEmbarkmentStatus > convoyEmbarkmentStatusInteractionSender;
        MockTransporters transporters;
        interactions::NetnOfferConvoy offer;
        interactions::NetnRequestConvoy request;
        tools::MessageController< sword::SimToClient_Content > messageController;
        MockCallsignResolver callsignResolver;
        TransportationOfferer offerConvoySender;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_sender_sends_offer_when_receiving_request_convoy_interaction_and_transporter_available, Fixture )
{
    request.serviceType = 4; // Convoy
    request.consumer = UnicodeString( "consumer" );
    request.provider = UnicodeString( "provider" );
    request.serviceId.eventCount = 42;
    request.serviceId.issuingObjectIdentifier = UnicodeString( "objectIdentifier" );
    request.transportData.convoyType = 0; // Transport
    request.transportData.dataTransport.appointment = NetnAppointmentStruct( 1, rpr::WorldLocation( 1., 2., 3. ) );
    request.transportData.dataTransport.finalAppointment = NetnAppointmentStruct( 2, rpr::WorldLocation( 4., 5., 6. ) );
    request.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( "transported", "unique", NetnObjectFeatureStruct() ) );
    MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( "uniqueV" ).returns( 111 );
    MOCK_EXPECT( transporters, Apply ).once().with( "unique", mock::any, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _3, "vessel", "uniqueV" ) );
    MOCK_EXPECT( offerInteractionSender, Send ).once().with( mock::retrieve( offer ) );
    offerConvoySender.Receive( request );
    mock::verify();
    BOOST_CHECK_EQUAL( offer.isOffering                       , 1 ); // true
    BOOST_CHECK_EQUAL( offer.requestTimeOut                   , 0u ); // no timeout
    BOOST_CHECK_EQUAL( offer.serviceId.eventCount             , request.serviceId.eventCount );
    BOOST_CHECK_EQUAL( offer.serviceId.issuingObjectIdentifier.str(), request.serviceId.issuingObjectIdentifier.str() );
    BOOST_CHECK_EQUAL( offer.consumer.str()                   , request.consumer.str() );
    BOOST_CHECK_EQUAL( offer.provider.str()                   , request.provider.str() );
    BOOST_CHECK_EQUAL( offer.serviceType                      , request.serviceType );
    BOOST_CHECK_EQUAL( offer.offerType                        , 1 ); // RequestAcknowledgePositive
    BOOST_CHECK_EQUAL( offer.listOfTransporters.list.size(), 1u );
    BOOST_CHECK_EQUAL( offer.listOfTransporters.list[ 0 ].callsign.str(), "vessel" );
    BOOST_CHECK_EQUAL( offer.listOfTransporters.list[ 0 ].uniqueId.str(), "uniqueV" );
    BOOST_CHECK_EQUAL( offer.transportData.convoyType, request.transportData.convoyType );
    BOOST_CHECK_EQUAL( offer.transportData.dataTransport.objectToManage.size(),
                       request.transportData.dataTransport.objectToManage.size() );
    BOOST_CHECK_EQUAL( offer.transportData.dataTransport.appointment.dateTime,
                       request.transportData.dataTransport.appointment.dateTime );
    BOOST_CHECK_CLOSE( offer.transportData.dataTransport.appointment.location.Latitude(),
                       request.transportData.dataTransport.appointment.location.Latitude(), 0.00001 );
    BOOST_CHECK_CLOSE( offer.transportData.dataTransport.appointment.location.Longitude(),
                       request.transportData.dataTransport.appointment.location.Longitude(), 0.00001 );
    BOOST_CHECK_EQUAL( offer.transportData.dataTransport.finalAppointment.dateTime,
                       request.transportData.dataTransport.finalAppointment.dateTime );
    BOOST_CHECK_CLOSE( offer.transportData.dataTransport.finalAppointment.location.Latitude(),
                       request.transportData.dataTransport.finalAppointment.location.Latitude(), 0.00001 );
    BOOST_CHECK_CLOSE( offer.transportData.dataTransport.finalAppointment.location.Longitude(),
                       request.transportData.dataTransport.finalAppointment.location.Longitude(), 0.00001 );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_no_transporter_available, Fixture )
{
    request.serviceType = 4; // Convoy
    request.transportData.convoyType = 0; // Transport
    request.transportData.dataTransport.appointment.location = rpr::WorldLocation( 1., 2., 3. );
    request.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( "transported", "unique", NetnObjectFeatureStruct() ) );
    MOCK_EXPECT( transporters, Apply ).once().with( "unique", mock::any, mock::any );
    offerConvoySender.Receive( request );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_request_is_not_convoy_type, Fixture )
{
    request.serviceType = 3; // Storage
    request.transportData.convoyType = 0; // Transport
    request.transportData.dataTransport.appointment.location = rpr::WorldLocation( 1., 2., 3. );
    request.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( "transported", "unique", NetnObjectFeatureStruct() ) );
    offerConvoySender.Receive( request );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_request_is_not_transport, Fixture )
{
    request.serviceType = 4; // Convoy
    request.transportData.convoyType = 1; // Embarkment
    request.transportData.dataTransport.appointment.location = rpr::WorldLocation( 1., 2., 3. );
    request.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( "transported", "unique", NetnObjectFeatureStruct() ) );
    offerConvoySender.Receive( request );
}

namespace
{
    class SentFixture : public Fixture
    {
    public:
        SentFixture()
        {
            request.serviceType = 4; // Convoy
            request.consumer = UnicodeString( "consumer" );
            request.provider = UnicodeString( "provider" );
            request.serviceId.eventCount = 42;
            request.serviceId.issuingObjectIdentifier = UnicodeString( "SWORD" );
            request.transportData.convoyType = 0; // Transport
            request.transportData.dataTransport.appointment = NetnAppointmentStruct( 1, rpr::WorldLocation( 1., 2., 3. ) );
            request.transportData.dataTransport.finalAppointment = NetnAppointmentStruct( 2, rpr::WorldLocation( 4., 5., 6. ) );
            request.transportData.dataTransport.objectToManage.push_back( NetnObjectDefinitionStruct( "transported", "unique", NetnObjectFeatureStruct() ) );
            MOCK_EXPECT( callsignResolver, ResolveSimulationIdentifier ).once().with( "uniqueV" ).returns( 111 );
            MOCK_EXPECT( transporters, Apply ).once().with( "unique", mock::any, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _3, "vessel", "uniqueV" ) );
            MOCK_EXPECT( offerInteractionSender, Send ).once().with( mock::retrieve( offer ) );
            offerConvoySender.Receive( request );
            mock::verify();
        }
        interactions::NetnAcceptOffer accept;
        interactions::NetnReadyToReceiveService readyToReceive;
        interactions::NetnServiceStarted serviceStarted;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_sends_service_started_when_accept_offer_and_ready_to_receive_service_received, SentFixture )
{
    accept.serviceId = offer.serviceId;
    readyToReceive.serviceId = offer.serviceId;
    MOCK_EXPECT( serviceStartedInteractionSender, Send ).once().with( mock::retrieve( serviceStarted ) );
    offerConvoySender.Receive( accept );
    offerConvoySender.Receive( readyToReceive );
    mock::verify();
    BOOST_CHECK_EQUAL( serviceStarted.serviceId.eventCount, offer.serviceId.eventCount );
    BOOST_CHECK_EQUAL( serviceStarted.serviceId.issuingObjectIdentifier.str(), offer.serviceId.issuingObjectIdentifier.str() );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_accept_offer_event_count_is_unkown, SentFixture )
{
    accept.serviceId.eventCount = offer.serviceId.eventCount + 1;
    accept.serviceId.issuingObjectIdentifier = offer.serviceId.issuingObjectIdentifier;
    offerConvoySender.Receive( accept );
    readyToReceive.serviceId = offer.serviceId;
    offerConvoySender.Receive( readyToReceive );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_ready_to_receive_service_event_count_is_unkown, SentFixture )
{
    accept.serviceId = offer.serviceId;
    offerConvoySender.Receive( accept );
    readyToReceive.serviceId.eventCount = offer.serviceId.eventCount + 1;
    readyToReceive.serviceId.issuingObjectIdentifier = offer.serviceId.issuingObjectIdentifier;
    offerConvoySender.Receive( readyToReceive );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_accept_offer_issuing_object_identifier_is_unkown, SentFixture )
{
    accept.serviceId.eventCount = offer.serviceId.eventCount;
    accept.serviceId.issuingObjectIdentifier = UnicodeString( "unknown identifier" );
    offerConvoySender.Receive( accept );
    readyToReceive.serviceId = offer.serviceId;
    offerConvoySender.Receive( readyToReceive );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_nothing_if_ready_to_receive_service_issuing_object_identifier_is_unkown, SentFixture )
{
    accept.serviceId = offer.serviceId;
    offerConvoySender.Receive( accept );
    readyToReceive.serviceId.eventCount = offer.serviceId.eventCount;
    readyToReceive.serviceId.issuingObjectIdentifier = UnicodeString( "unknown identifier" );;
    offerConvoySender.Receive( readyToReceive );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_does_not_resend_service_started_when_receiving_ready_to_receive_service, SentFixture )
{
    accept.serviceId = offer.serviceId;
    readyToReceive.serviceId = offer.serviceId;
    MOCK_EXPECT( serviceStartedInteractionSender, Send ).once().with( mock::retrieve( serviceStarted ) );
    offerConvoySender.Receive( accept );
    offerConvoySender.Receive( readyToReceive );
    mock::verify();
    offerConvoySender.Receive( readyToReceive );
}

namespace
{
    class StartedFixture : public SentFixture
    {
    public:
        StartedFixture()
        {
            accept.serviceId = offer.serviceId;
            readyToReceive.serviceId = offer.serviceId;
            MOCK_EXPECT( serviceStartedInteractionSender, Send ).once();
            offerConvoySender.Receive( accept );
            offerConvoySender.Receive( readyToReceive );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_sends_convoy_embarkment_status_when_transporting_unit_embark_units, StartedFixture )
{
    const unsigned int transporterId = 111;
    const unsigned int transportedId = 222;
    sword::SimToClient_Content message;
    sword::UnitAttributes* attributes = message.mutable_unit_attributes();
    attributes->mutable_unit()->set_id( transporterId );
    attributes->mutable_transported_units()->add_elem()->set_id( transportedId );
    interactions::NetnConvoyEmbarkmentStatus status;
    MOCK_EXPECT( callsignResolver, ResolveCallsign ).once().with( transportedId ).returns( "transported" );
    MOCK_EXPECT( callsignResolver, ResolveUniqueId ).once().with( transportedId ).returns( "unique" );
    MOCK_EXPECT( convoyEmbarkmentStatusInteractionSender, Send ).once().with( mock::retrieve( status ) );
    messageController.Dispatch( message );
    BOOST_CHECK_EQUAL( status.serviceId.eventCount, 42 );
    BOOST_CHECK_EQUAL( status.listOfObjectEmbarked.list.size(), 1u );
    BOOST_CHECK_EQUAL( status.listOfObjectEmbarked.list[ 0 ].callsign.str(), "transported" );
    BOOST_CHECK_EQUAL( status.listOfObjectEmbarked.list[ 0 ].uniqueId.str(), "unique" );
    BOOST_CHECK_EQUAL( status.transportUnitIdentifier.str(), "vessel" );
    mock::verify();
    attributes->mutable_unit()->set_id( transporterId + 1 );
    messageController.Dispatch( message );
}
