// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnOfferConvoyReceiver.h"
#include "hla_plugin/Interactions.h"
#include "MockTransportationController.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : receiver( controller )
        {}
        interactions::NetnOfferConvoy MakeOffer( unsigned int context, bool offering, unsigned int offerType, int8 serviceType,
                                                 unsigned int convoyType, const std::string& issuingObjectIdentifier,
                                                 const std::string& provider )
        {
            interactions::NetnOfferConvoy offer;
            offer.isOffering = offering;
            offer.offerType = offerType;
            offer.serviceId = NetnEventIdentifier( context, issuingObjectIdentifier );
            offer.serviceType = serviceType;
            offer.transportData.convoyType = convoyType;
            offer.provider = UnicodeString( provider );
            return offer;
        }
        MockTransportationController controller;
        NetnOfferConvoyReceiver receiver;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_receiver_notifies_listener_if_offer_is_valid, Fixture )
{
    MOCK_EXPECT( controller, OfferReceived ).once().with( 42u, true, "provider" );
    interactions::NetnOfferConvoy offer = MakeOffer( 42, true, 1, 4, 0, "SWORD", "provider" );
    receiver.Receive( offer );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_receiver_notifies_listener_with_partial, Fixture )
{
    {
        MOCK_EXPECT( controller, OfferReceived ).once().with( 42u, false, "provider" );
        interactions::NetnOfferConvoy offer = MakeOffer( 42, false, 1, 4, 0, "SWORD", "provider" );
        receiver.Receive( offer );
    }
    {
        MOCK_EXPECT( controller, OfferReceived ).once().with( 42u, false, "provider" );
        interactions::NetnOfferConvoy offer = MakeOffer( 42, true, 0, 4, 0, "SWORD", "provider" );
        receiver.Receive( offer );
    }
    {
        MOCK_EXPECT( controller, OfferReceived ).once().with( 42u, false, "provider" );
        interactions::NetnOfferConvoy offer = MakeOffer( 42, true, 2, 4, 0, "SWORD", "provider" );
        receiver.Receive( offer );
    }
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_receiver_does_nothing_if_issuing_object_identifier_is_not_sword, Fixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 42, true, 1, 4, 0, "ORQUE", "provider" );
    receiver.Receive( offer );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_receiver_does_nothing_if_service_type_is_not_valid, Fixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 42, true, 1, 1, 0, "SWORD", "provider" );
    receiver.Receive( offer );
}

BOOST_FIXTURE_TEST_CASE( netn_offer_convoy_receiver_does_nothing_if_convoy_type_is_not_transport, Fixture )
{
    interactions::NetnOfferConvoy offer = MakeOffer( 42, true, 1, 1, 1, "SWORD", "provider" );
    receiver.Receive( offer );
}
