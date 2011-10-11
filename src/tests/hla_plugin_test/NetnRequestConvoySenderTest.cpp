// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnRequestConvoySender.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/TransportedUnitsVisitor_ABC.h"
#include "MockTransportationController.h"
#include "MockInteractionSender.h"
#include "MockContextFactory.h"
#include "MockTransportedUnits.h"

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( netn_request_convoy_sender_sends_request_when_receiving_convoy_event )
{
    MockContextFactory contextFactory;
    MockTransportationController controller;
    MockInteractionSender< interactions::NetnRequestConvoy > interactionSender;
    TransportationListener_ABC* listener = 0;
    MOCK_EXPECT( controller, Register ).once().with( mock::retrieve( listener ) );
    MOCK_EXPECT( controller, Unregister );
    NetnRequestConvoySender sender( controller, interactionSender, contextFactory );
    BOOST_REQUIRE( listener );
    interactions::NetnRequestConvoy convoy;
    MOCK_EXPECT( contextFactory, Create ).once().returns( 42 );
    MOCK_EXPECT( interactionSender, Send ).once().with( mock::retrieve( convoy ) );
    const long long embarkmentTime = 1337;
    const long long disembarkmentTime = 1338;
    const geometry::Point2d embarkmentPoint( 1., 2. );
    const geometry::Point2d disembarkmentPoint( 3., 4. );
    MockTransportedUnits transportedUnits;
    const std::string callsign = "callsign";
    const std::string uniqueId = "uniqueId";
    MOCK_EXPECT( transportedUnits, Apply ).once().calls( boost::bind( &TransportedUnitsVisitor_ABC::Notify, _1, callsign, uniqueId ) );
    listener->ConvoyRequested( "carrier-identifier", embarkmentTime, embarkmentPoint, disembarkmentTime, disembarkmentPoint, transportedUnits );
    mock::verify();
    const int convoyServiceType = 4;
    const unsigned int noTimeout = 0;
    const int convoyTransportType = 0;
    const int noDetail = 0;
    BOOST_CHECK_EQUAL( convoy.serviceId.eventCount, 42 );
    BOOST_CHECK_EQUAL( convoy.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.consumer.str(), "SWORD" );
    BOOST_CHECK_EQUAL( convoy.provider.str(), "carrier-identifier" );
    BOOST_CHECK_EQUAL( convoy.serviceType, convoyServiceType );
    BOOST_CHECK_EQUAL( convoy.requestTimeOut, noTimeout );
    BOOST_CHECK_EQUAL( convoy.transportData.convoyType, convoyTransportType );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.appointment.dateTime, embarkmentTime );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.appointment.location.Latitude(), embarkmentPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.appointment.location.Longitude(), embarkmentPoint.Y(), 0.00001 );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.finalAppointment.dateTime, disembarkmentTime );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.finalAppointment.location.Latitude(), disembarkmentPoint.X(), 0.00001 );
    BOOST_CHECK_CLOSE( convoy.transportData.dataTransport.finalAppointment.location.Longitude(), disembarkmentPoint.Y(), 0.00001 );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage.size(), 1u );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage[ 0 ].callsign.str(), callsign );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage[ 0 ].uniqueId.str(), uniqueId );
    BOOST_CHECK_EQUAL( convoy.transportData.dataTransport.objectToManage[ 0 ].objectFeature.featureLevel, noDetail );
}
