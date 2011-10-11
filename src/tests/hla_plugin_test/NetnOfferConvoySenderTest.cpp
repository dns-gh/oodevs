// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnOfferConvoySender.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/TransportedUnitsVisitor_ABC.h"
#include "MockTransportationController.h"
#include "MockInteractionSender.h"
#include "MockContextFactory.h"
#include "MockTransportedUnits.h"
#include "MockTransporters.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : offerConvoySender( interactionSender, transporters )
        {}
        MockInteractionSender< interactions::NetnOfferConvoy > interactionSender;
        MockTransporters transporters;
        interactions::NetnOfferConvoy offer;
        interactions::NetnRequestConvoy request;
        NetnOfferConvoySender offerConvoySender;
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
    MOCK_EXPECT( transporters, Apply ).once().with( "unique", mock::any, mock::any ).calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _3, "vessel", "uniqueV" ) );
    MOCK_EXPECT( interactionSender, Send ).once().with( mock::retrieve( offer ) );
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
    BOOST_CHECK_EQUAL( offer.listOfTransporters.listOfTransporters.size(), 1u );
    BOOST_CHECK_EQUAL( offer.listOfTransporters.listOfTransporters[ 0 ].callsign.str(), "vessel" );
    BOOST_CHECK_EQUAL( offer.listOfTransporters.listOfTransporters[ 0 ].uniqueId.str(), "uniqueV" );
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
