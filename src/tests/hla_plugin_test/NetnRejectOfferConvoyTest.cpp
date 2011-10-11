// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnRejectOfferConvoy.h"
#include "hla_plugin/Interactions.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( netn_reject_offer_convoy_registration_publish_only )
{
    MockFederate federate;
    hla::MockInteractionHandler* handler = new hla::MockInteractionHandler(); // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
    MOCK_EXPECT( federate, RegisterInteraction ).once().with( "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy", mock::any, true, false ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );
    NetnRejectOfferConvoy interaction( federate );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : handler( new hla::MockInteractionHandler() ) // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
        {
            MOCK_EXPECT( federate, RegisterInteraction ).once().calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );;
        }
        MockFederate federate;
        hla::MockInteractionHandler* handler;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : interaction( federate )
        {
            // NOTHING
        }
        NetnRejectOfferConvoy interaction;
        mock::sequence s;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_reject_offer_convoy_is_sent_with_attributes, RegisteredFixture )
{
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    BOOST_FOREACH( const std::string& parameter, parameters )
        MOCK_EXPECT( handler, Visit ).once().in( s ).with( parameter, mock::any );
    MOCK_EXPECT( handler, End ).once().in( s );
    interactions::NetnRejectOfferConvoy offerConvoy;
    interaction.Send( offerConvoy );
}
