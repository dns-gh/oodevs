// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnOfferResponseSender.h"
#include "hla_plugin/Interactions.h"
#include "MockInteractionSender.h"
#include "MockTransportationController.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : transportationListener( 0 )
            , context               ( 42 )
        {
            MOCK_EXPECT( controller, Register ).once().with( mock::retrieve( transportationListener ) );
            MOCK_EXPECT( controller, Unregister ).once();
        }
        MockInteractionSender< interactions::NetnAcceptOffer > acceptOfferSender;
        MockInteractionSender< interactions::NetnRejectOfferConvoy > rejectOfferSender;
        MockTransportationController controller;
        TransportationListener_ABC* transportationListener;
        const int context;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_offer_response_sender_sends_accept_offer_when_notified, Fixture )
{
    NetnOfferResponseSender sender( controller, acceptOfferSender, rejectOfferSender );
    BOOST_REQUIRE( transportationListener );
    interactions::NetnAcceptOffer acceptation;
    MOCK_EXPECT( acceptOfferSender, Send ).once().with( mock::retrieve( acceptation ) );
    transportationListener->OfferAccepted( context, "provider" );
    BOOST_CHECK_EQUAL( acceptation.serviceId.eventCount, context );
    BOOST_CHECK_EQUAL( acceptation.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( acceptation.consumer.str(), "SWORD" );
    BOOST_CHECK_EQUAL( acceptation.provider.str(), "provider" );
    BOOST_CHECK_EQUAL( acceptation.serviceType, 4 );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_response_sender_sends_reject_offer_when_notified, Fixture )
{
    NetnOfferResponseSender sender( controller, acceptOfferSender, rejectOfferSender );
    BOOST_REQUIRE( transportationListener );
    interactions::NetnRejectOfferConvoy rejection;
    MOCK_EXPECT( rejectOfferSender, Send ).once().with( mock::retrieve( rejection ) );
    transportationListener->OfferRejected( context, "provider", "reason" );
    BOOST_CHECK_EQUAL( rejection.serviceId.eventCount, context );
    BOOST_CHECK_EQUAL( rejection.serviceId.issuingObjectIdentifier.str(), "SWORD" );
    BOOST_CHECK_EQUAL( rejection.consumer.str(), "SWORD" );
    BOOST_CHECK_EQUAL( rejection.provider.str(), "provider" );
    BOOST_CHECK_EQUAL( rejection.serviceType, 4 );
    BOOST_CHECK_EQUAL( rejection.reason, "reason" );
}
