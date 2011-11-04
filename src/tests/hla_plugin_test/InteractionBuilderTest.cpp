// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/InteractionBuilder.h"
#include "hla_plugin/Interactions.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockInteractionNotification.h"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        template< typename Interaction >
        void CheckBuild( const std::string& name, const std::vector< std::string >& parameters )
        {
            ::hla::MockInteractionNotification< Interaction > notification;
            ::hla::Interaction< Interaction > interaction( notification );
            MockFederate federate;
            hla::MockInteractionHandler* handler = new hla::MockInteractionHandler();
            MOCK_EXPECT( federate, RegisterInteraction ).once().with( name, mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );
            builder.Build( federate, interaction );
            mock::verify();
            mock::sequence s;
            BOOST_FOREACH( const std::string& parameter, parameters )
                MOCK_EXPECT( handler, Visit ).once().in( s ).with( parameter, mock::any );
            MOCK_EXPECT( handler, End ).once().in( s );
            Interaction message;
            interaction.Send( message );
        }
        InteractionBuilder builder;
    };
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_request_convoy, Fixture )
{
    const std::string name = "NETN_Service.NETN_RequestService.NETN_RequestConvoy";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "RequestTimeOut" )
                                                                        ( "TransportData" );
    CheckBuild< interactions::NetnRequestConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_offer_convoy, Fixture )
{
    const std::string name = "NETN_Service.NETN_OfferService.NETN_OfferConvoy";
    const std::vector< std::string > parameters = boost::assign::list_of( "IsOffering" )
                                                                        ( "RequestTimeOut" )
                                                                        ( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "TransportData" )
                                                                        ( "OfferType" )
                                                                        ( "ListOfTransporters" );
    CheckBuild< interactions::NetnOfferConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_accept_offer, Fixture )
{
    const std::string name = "NETN_Service.NETN_AcceptOffer";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnAcceptOffer>( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_reject_offer_convoy, Fixture )
{
    const std::string name = "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    CheckBuild< interactions::NetnRejectOfferConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_cancel_convoy, Fixture )
{
    const std::string name = "NETN_Service.NETN_CancelService.NETN_CancelConvoy";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    CheckBuild< interactions::NetnCancelConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_ready_to_receive_service, Fixture )
{
    const std::string name = "NETN_Service.NETN_ReadyToReceiveService";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnReadyToReceiveService >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_started, Fixture )
{
    const std::string name = "NETN_Service.NETN_ServiceStarted";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceStarted >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_embarkment_status, Fixture )
{
    const std::string name = "NETN_Service.NETN_ConvoyEmbarkmentStatus";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "ListOfObjectEmbarked" )
                                                                        ( "TransportUnitIdentifier" );
    CheckBuild< interactions::NetnConvoyEmbarkmentStatus >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_disembarkment_status, Fixture )
{
    const std::string name = "NETN_Service.NETN_ConvoyDisembarkmentStatus";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "ListOfObjectDisembarked" )
                                                                        ( "TransportUnitIdentifier" );
    CheckBuild< interactions::NetnConvoyDisembarkmentStatus >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_destroyed_entities, Fixture )
{
    const std::string name = "NETN_Service.NETN_ConvoyDestroyedEntities";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "ListOfEmbarkedObjectDestroyed" );
    CheckBuild< interactions::NetnConvoyDestroyedEntities >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_complete, Fixture )
{
    const std::string name = "NETN_Service.NETN_ServiceComplete";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceComplete >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_received, Fixture )
{
    const std::string name = "NETN_Service.NETN_ServiceReceived";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceReceived >( name, parameters );
}
