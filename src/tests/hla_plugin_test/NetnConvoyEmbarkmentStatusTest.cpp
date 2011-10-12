// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/NetnConvoyEmbarkmentStatus.h"
#include "hla_plugin/Interactions.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockInteractionNotification.h"
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( netn_convoy_embarkment_status_registration_publish_subscribe )
{
    MockFederate federate;
    ::hla::MockInteractionNotification< interactions::NetnConvoyEmbarkmentStatus > notification;
    hla::MockInteractionHandler* handler = new hla::MockInteractionHandler(); // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
    MOCK_EXPECT( federate, RegisterInteraction ).once().with( "NETN_Service.NETN_ConvoyEmbarkmentStatus", mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );
    NetnConvoyEmbarkmentStatus interaction( federate, notification );
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
        ::hla::MockInteractionNotification< interactions::NetnConvoyEmbarkmentStatus > notification;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        RegisteredFixture()
            : interaction( federate, notification )
        {
            // NOTHING
        }
       NetnConvoyEmbarkmentStatus interaction;
        mock::sequence s;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_convoy_embarkment_status_is_sent_with_attributes, RegisteredFixture )
{
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "ListOfObjectEmbarked" )
                                                                        ( "TransportUnitIdentifier" );
                                                                        ( "ListOfTransporters" );
    BOOST_FOREACH( const std::string& parameter, parameters )
        MOCK_EXPECT( handler, Visit ).once().in( s ).with( parameter, mock::any );
    MOCK_EXPECT( handler, End ).once().in( s );
    interactions::NetnConvoyEmbarkmentStatus offerConvoy;
    interaction.Send( offerConvoy );
}
