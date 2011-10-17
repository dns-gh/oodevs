// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnConvoyEmbarkmentStatusReceiver.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/TransportedUnits_ABC.h"
#include "MockTransportationController.h"
#include "MockTransportedUnitsVisitor.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : receiver( controller )
        {}
        MockTransportationController controller;
        NetnConvoyEmbarkmentStatusReceiver receiver;
    };
    bool Check( const TransportedUnits_ABC& units, const std::string& callsign, const std::string& id )
    {
        MockTransportedUnitsVisitor visitor;
        MOCK_EXPECT( visitor, Notify ).once().with( callsign, id );
        units.Apply( visitor );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( netn_convoy_embarkment_status_receiver_notifies_listener_if_status_is_valid, Fixture )
{
    MOCK_EXPECT( controller, NotifyEmbarkationStatus ).once().with( 42u, "vessel", boost::bind( &Check, _1, "callsign", "uniqueId" ) );
    interactions::NetnConvoyEmbarkmentStatus offer;
    offer.serviceId = NetnEventIdentifier( 42, "SWORD" );
    offer.serviceType = 4;
    offer.transportUnitIdentifier = UnicodeString( "vessel" );
    offer.listOfObjectEmbarked.list.push_back( NetnObjectDefinitionStruct( "callsign", "uniqueId", NetnObjectFeatureStruct() ) );
    receiver.Receive( offer );
}

BOOST_FIXTURE_TEST_CASE( netn_convoy_embarkment_status_receiver_does_nothing_if_issuing_object_identifier_is_not_sword, Fixture )
{
    interactions::NetnConvoyEmbarkmentStatus offer;
    offer.serviceId = NetnEventIdentifier( 42, "ORQUE" );
    offer.serviceType = 4;
    receiver.Receive( offer );
}

BOOST_FIXTURE_TEST_CASE( netn_convoy_embarkment_status_receiver_does_nothing_if_convoy_type_is_not_transport, Fixture )
{
    interactions::NetnConvoyEmbarkmentStatus offer;
    offer.serviceId = NetnEventIdentifier( 42, "SWORD" );
    offer.serviceType = 5;
    receiver.Receive( offer );
}
