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
#include "hla_plugin/InteractionSender.h"
#include "hla_plugin/Interactions.h"
#include "hla_plugin/NETNv1_InteractionBuilder.h"
#include "hla_plugin/NETNv2_InteractionBuilder.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockInteractionNotification.h"
#include "MockLogger.h"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    template <typename NetnBldr >
    class FixtureBase
    {
    public:
        FixtureBase()
            : netnBuilder( logger, federate )
            , builder( logger, federate, netnBuilder )
        {}
        template< typename Interaction >
        void CheckBuild( const std::string& name, const std::vector< std::string >& parameters, const Interaction& message = Interaction() )
        {
            ::hla::MockInteractionNotification< Interaction > notification;
            ::hla::Interaction< Interaction > interaction( notification );
            hla::MockInteractionHandler* handler = new hla::MockInteractionHandler();
            std::auto_ptr< ::hla::InteractionHandler_ABC > handlerPtr( handler );
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( name, mock::any, true, true ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( handlerPtr ) ) );
            BOOST_CHECK( builder.Build( interaction ) );
            mock::verify();
            mock::sequence s;
            BOOST_FOREACH( const std::string& parameter, parameters )
                MOCK_EXPECT( handler->Visit ).once().in( s ).with( parameter, mock::any );
            MOCK_EXPECT( handler->End ).once().in( s );
            interaction.Send( message );
        }
        template< typename Interaction >
        void CheckBuildFailure( const std::string& name )
        {
            ::hla::MockInteractionNotification< Interaction > notification;
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( name, mock::any, true, true ).throws( ::hla::HLAException("error") );
            MOCK_EXPECT( logger.LogError ).once();
            InteractionSender< Interaction > interaction( notification, builder );
            BOOST_CHECK( !interaction.IsSupported() );
            mock::verify();
        }
        MockFederate federate;
        dispatcher::MockLogger logger;
        NetnBldr netnBuilder;
        InteractionBuilder builder;
    };
    typedef FixtureBase< NETNv1_InteractionBuilder > Fixture;
    typedef FixtureBase< NETNv2_InteractionBuilder > FixtureV2;
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_acknowledge, Fixture )
{
    const std::string name = "Acknowledge";
    const std::vector< std::string > parameters = boost::assign::list_of( "OriginatingEntity" )
                                                                        ( "ReceivingEntity" )
                                                                        ( "RequestIdentifier" )
                                                                        ( "AcknowledgeFlag" )
                                                                        ( "ResponseFlag" );

    CheckBuild< interactions::Acknowledge >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_munition_detonation, Fixture )
{
    const std::string name = "MunitionDetonation";
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
    CheckBuild< interactions::MunitionDetonation >( name, parameters );
    CheckBuildFailure< interactions::MunitionDetonation >( name );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_service_request, Fixture )
{
    const std::string name = "ServiceRequest";
    const std::vector< std::string > parameters = boost::assign::list_of( "RequestingObject" )
                                                                        ( "ServicingObject" )
                                                                        ( "ServiceType" )
                                                                        ( "SuppliesData" );
    CheckBuild< interactions::ServiceRequest >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_resupply_cancel, Fixture )
{
    const std::string name = "ResupplyCancel";
    const std::vector< std::string > parameters = boost::assign::list_of( "ReceivingObject" )
                                                                        ( "SupplyingObject" );
    CheckBuild< interactions::ResupplyCancel >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_resupply_offer, Fixture )
{
    const std::string name = "ResupplyOffer";
    const std::vector< std::string > parameters = boost::assign::list_of( "ReceivingObject" )
                                                                        ( "SupplyingObject" )
                                                                        ( "SuppliesData" );
    CheckBuild< interactions::ResupplyOffer >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_resupply_received, Fixture )
{
    const std::string name = "ResupplyReceived";
    const std::vector< std::string > parameters = boost::assign::list_of( "ReceivingObject" )
                                                                        ( "SupplyingObject" )
                                                                        ( "SuppliesData" );
    CheckBuild< interactions::ResupplyReceived >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_transfer_control, Fixture )
{
    const std::string name = "TransferControl";
    const std::vector< std::string > parameters = boost::assign::list_of( "OriginatingEntity" )
                                                                        ( "ReceivingEntity" )
                                                                        ( "RequestIdentifier" )
                                                                        ( "TransferType" )
                                                                        ( "TransferEntity" )
                                                                        ( "RecordSetData" );
    CheckBuild< interactions::TransferControl >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_comment, Fixture )
{
    const std::string name = "Comment";
    const std::vector< std::string > parameters = boost::assign::list_of( "OriginatingEntity" )
                                                                        ( "ReceivingEntity" )
                                                                        ( "VariableDatumSet" );
    CheckBuild< interactions::Comment >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_request_convoy_v1, Fixture )
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

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_offer_convoy_v1, Fixture )
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

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_accept_offer_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_AcceptOffer";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnAcceptOffer>( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_reject_offer_convoy_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    CheckBuild< interactions::NetnRejectOfferConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_cancel_convoy_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_CancelService.NETN_CancelConvoy";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    CheckBuild< interactions::NetnCancelConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_ready_to_receive_service_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_ReadyToReceiveService";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnReadyToReceiveService >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_started_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_ServiceStarted";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceStarted >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_embarkment_status_v1, Fixture )
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

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_disembarkment_status_v1, Fixture )
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

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_destroyed_entities_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_ConvoyDestroyedEntities";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "ListOfEmbarkedObjectDestroyed" );
    CheckBuild< interactions::NetnConvoyDestroyedEntities >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_complete_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_ServiceComplete";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceComplete >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_received_v1, Fixture )
{
    const std::string name = "NETN_Service.NETN_ServiceReceived";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceReceived >( name, parameters );
}

// NETNv2

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_request_convoy_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_RequestService.RequestTransport";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "RequestTimeOut" )
                                                                        ( "TransportData" );
    CheckBuild< interactions::NetnRequestConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_offer_convoy_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_OfferService.OfferTransport";
    const std::vector< std::string > parameters = boost::assign::list_of( "IsOffering" )
                                                                        ( "RequestTimeOut" )
                                                                        ( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "TransportData" )
                                                                        ( "OfferType" )
                                                                        ( "Transporters" );
    CheckBuild< interactions::NetnOfferConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_accept_offer_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_AcceptOffer";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnAcceptOffer>( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_reject_offer_convoy_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_RejectOffer";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    CheckBuild< interactions::NetnRejectOfferConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_cancel_convoy_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_CancelService";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "Reason" );
    CheckBuild< interactions::NetnCancelConvoy >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_ready_to_receive_service_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_ReadyToReceiveService";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnReadyToReceiveService >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_started_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_ServiceStarted";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceStarted >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_embarkment_status_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.TransportEmbarkmentStatus";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "EmbarkedObjects" )
                                                                        ( "TransportUnitIdentifier" );
    interactions::NetnConvoyEmbarkmentStatus data;
    data.transportUnitIdentifier = UnicodeString("D019EF26-9A59-47C4-8775-4DE8D4AF9FE3");
    CheckBuild< interactions::NetnConvoyEmbarkmentStatus >( name, parameters, data );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_disembarkment_status_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.TransportDisembarkmentStatus";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "DisembarkedObjects" )
                                                                        ( "TransportUnitIdentifier" );
    interactions::NetnConvoyDisembarkmentStatus data;
    data.transportUnitIdentifier = UnicodeString("D019EF26-9A59-47C4-8775-4DE8D4AF9FE3");
    CheckBuild< interactions::NetnConvoyDisembarkmentStatus >( name, parameters, data );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_convoy_destroyed_entities_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.TransportDestroyedEntities";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" )
                                                                        ( "DestroyedObjects" );
    CheckBuild< interactions::NetnConvoyDestroyedEntities >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_complete_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_ServiceComplete";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceComplete >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( transportation_interaction_builder_registers_name_and_attributes_for_netn_service_received_v2, FixtureV2 )
{
    const std::string name = "SCP_Service.SCP_ServiceReceived";
    const std::vector< std::string > parameters = boost::assign::list_of( "ServiceID" )
                                                                        ( "Consumer" )
                                                                        ( "Provider" )
                                                                        ( "ServiceType" );
    CheckBuild< interactions::NetnServiceReceived >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_offer_transfer_modelling_responibility, FixtureV2 )
{
    const std::string name = "TMR.TMR_OfferTransferModellingResponsibility";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "RequestFederate" )
                                                                        ( "ResponseFederate" )
                                                                        ( "isOffering" )
                                                                        ( "Reason" );
    CheckBuild< interactions::TMR_OfferTransferModellingResponsibility >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_initiate_transfer_modelling_responibility, FixtureV2 )
{
    const std::string name = "TMR.TMR_InitiateTransferModellingResponsibility";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "RequestFederate" )
                                                                        ( "ResponseFederate" )
                                                                        ( "TransferType" )
                                                                        ( "Instances" )
                                                                        ( "Attributes" )
                                                                        ( "Initiating" )
                                                                        ( "CapabilityType" )
                                                                        ( "InstanceAttributeValues" );
    CheckBuild< interactions::TMR_InitiateTransferModellingResponsibility >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_request_transfer_modelling_responibility, FixtureV2 )
{
    const std::string name = "TMR.TMR_RequestTransferModellingResponsibility";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "RequestFederate" )
                                                                        ( "ResponseFederate" )
                                                                        ( "TransferType" )
                                                                        ( "Instances" )
                                                                        ( "Attributes" )
                                                                        ( "CapabilityType" )
                                                                        ( "InstanceAttributeValues" );
    CheckBuild< interactions::TMR_RequestTransferModellingResponsibility >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_tmr_cancel_request, FixtureV2 )
{
    const std::string name = "TMR.TMR_CancelRequest";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "RequestFederate" )
                                                                        ( "ResponseFederate" )
                                                                        ( "Reason" );
    CheckBuild< interactions::TMR_CancelRequest >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_tmr_transfer_result, FixtureV2 )
{
    const std::string name = "TMR.TMR_TransferResult";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "RequestFederate" )
                                                                        ( "ResponseFederate" )
                                                                        ( "TransferOk" );
    CheckBuild< interactions::TMR_TransferResult >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_disaggregationrequest, FixtureV2 )
{
    const std::string name = "MRM_Object.MRM_DisaggregationRequest";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "AggregateFederate" )
                                                                        ( "HigherResolutionFederate" )
                                                                        ( "AggregateUUID" )
                                                                        ( "AggregationState" )
                                                                        ( "UuidList" );
    CheckBuild< interactions::MRM_DisaggregationRequest >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_disaggregationresponse, FixtureV2 )
{
    const std::string name = "MRM_Object.MRM_DisaggregationResponse";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "AggregateFederate" )
                                                                        ( "HigherResolutionFederate" )
                                                                        ( "Acknowledge" )
                                                                        ( "NonComplianceReason" );
    CheckBuild< interactions::MRM_DisaggregationResponse >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_aggregationrequest, FixtureV2 )
{
    const std::string name = "MRM_Object.MRM_AggregationRequest";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "AggregateFederate" )
                                                                        ( "HigherResolutionFederate" )
                                                                        ( "UuidList" )
                                                                        ( "AggregateUuid" );
    CheckBuild< interactions::MRM_AggregationRequest >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_aggregationresponse, FixtureV2 )
{
    const std::string name = "MRM_Object.MRM_AggregationResponse";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "AggregateFederate" )
                                                                        ( "HigherResolutionFederate" )
                                                                        ( "Acknowledge" )
                                                                        ( "NonComplianceReason" );
    CheckBuild< interactions::MRM_AggregationResponse >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_cancelrequest, FixtureV2 )
{
    const std::string name = "MRM_Object.MRM_CancelRequest";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "AggregateFederate" )
                                                                        ( "HigherResolutionFederate" )
                                                                        ( "Reason" );
    CheckBuild< interactions::MRM_CancelRequest >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_actioncomplete, FixtureV2 )
{
    const std::string name = "MRM_Object.MRM_ActionComplete";
    const std::vector< std::string > parameters = boost::assign::list_of( "TransactionID" )
                                                                        ( "AggregateFederate" )
                                                                        ( "HigherResolutionFederate" )
                                                                        ( "CompletionResult" );
    CheckBuild< interactions::MRM_ActionComplete >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_trigger, FixtureV2 )
{
    const std::string name = "MRM_Trigger";
    const std::vector< std::string > parameters = boost::assign::list_of( "Instance" )
                                                                        ( "AggregationState" )
                                                                        ( "UuidList" );
    CheckBuild< interactions::MRM_Trigger >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( build_mrm_triggerresponse, FixtureV2 )
{
    const std::string name = "MRM_TriggerResponse";
    const std::vector< std::string > parameters = boost::assign::list_of( "Instance" )
                                                                        ( "TransactionID" );
    CheckBuild< interactions::MRM_TriggerResponse >( name, parameters );
}

BOOST_FIXTURE_TEST_CASE( interaction_builder_registers_name_and_attributes_for_weapon_fire, Fixture )
{
    const std::string name = "WeaponFire";
    const std::vector< std::string > parameters = boost::assign::list_of( "EventIdentifier" )
                                                                        ( "FireControlSolutionRange" )
                                                                        ( "FireMissionIndex" )
                                                                        ( "FiringLocation" )
                                                                        ( "FiringObjectIdentifier" )
                                                                        ( "FuseType" )
                                                                        ( "InitialVelocityVector" )
                                                                        ( "MunitionObjectIdentifier" )
                                                                        ( "MunitionType" )
                                                                        ( "QuantityFired" )
                                                                        ( "RateOfFire" )
                                                                        ( "TargetObjectIdentifier" )
                                                                        ( "WarheadType" );
    CheckBuild< interactions::WeaponFire >( name, parameters );
    CheckBuildFailure< interactions::WeaponFire >( name );
}
