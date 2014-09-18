// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NETNv2_InteractionBuilder.h"

#include "InteractionsNetn.h"
#include "Federate_ABC.h"
#include "Encodable.h"
#include "dispatcher/Logger_ABC.h"

#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace plugins::hla;

namespace
{

    template< typename T, typename R, typename Interaction >
    void Register( ::hla::Interaction< Interaction >& interaction, const std::string& name, const std::string& parameter, R T::* pMemberData, dispatcher::Logger_ABC& logger )
    {
        interaction.Register( parameter, *new Encodable< T, R >( pMemberData, name, parameter, logger ) );
    }
#   define REGISTER( parameter, member ) Register( interaction, name, parameter, member, logger_ )
#   define REGISTER_TEMPLATE( parameter, member, T ) Register< T >( interaction, name, parameter, member, logger_ )

    template< typename T >
    void RegisterNetnService( ::hla::Interaction< T >& interaction, const std::string& name, dispatcher::Logger_ABC& logger_ )
    {
        REGISTER_TEMPLATE( "ServiceID"  , &T::serviceId, T );
        REGISTER_TEMPLATE( "Consumer"   , &T::consumer, T );
        REGISTER_TEMPLATE( "Provider"   , &T::provider, T );
        REGISTER_TEMPLATE( "ServiceType", &T::serviceType, T );
    }

    template< typename T >
    void RegisterTMR( ::hla::Interaction< T >& interaction, const std::string& name, dispatcher::Logger_ABC& logger_ )
    {
        REGISTER_TEMPLATE( "TransactionID", &T::transactionID, T );
        REGISTER_TEMPLATE( "RequestFederate", &T::requestFederate, T );
        REGISTER_TEMPLATE( "ResponseFederate", &T::responseFederate, T );
    }

    template< typename T >
    void RegisterMRM( ::hla::Interaction< T >& interaction, const std::string& name, dispatcher::Logger_ABC& logger_ )
    {
        REGISTER_TEMPLATE( "TransactionID", &T::transactionID, T );
        REGISTER_TEMPLATE( "AggregateFederate", &T::aggregateFederate, T );
        REGISTER_TEMPLATE( "HigherResolutionFederate", &T::higherResolutionFederate, T );
    }

    NETN_UUID objectDef2uuid( const NetnObjectDefinitionStruct& v )
    {
        return NETN_UUID( v.uniqueId ) ;
    }
    NetnObjectDefinitionStruct uuid2ObjDef( const NETN_UUID& v )
    {
        NetnObjectDefinitionStruct retval;
        retval.uniqueId = v.data();
        return retval;
    }
    template< typename T>
    struct TransportDataEncoder : public ::hla::Encodable_ABC< T >
    {
        virtual void Decode( ::hla::Deserializer_ABC& deserializer, T& object ) const
        {
            Netn2TransportStruct tmp;
            tmp.Deserialize( deserializer );
            object.transportData.convoyType = static_cast< NetnTransportStruct::ConvoyType >( tmp.transportType );
            switch( tmp.transportType )
            {
            case Netn2TransportStruct::E_Transport:
                object.transportData.dataTransport.appointment = tmp.dataTransport.appointment;
                object.transportData.dataTransport.finalAppointment = tmp.dataTransport.finalAppointment;
                object.transportData.dataTransport.objectToManage.resize( tmp.dataTransport.objectToManage.size() );
                std::transform( tmp.dataTransport.objectToManage.begin(), tmp.dataTransport.objectToManage.end(),
                    object.transportData.dataTransport.objectToManage.begin(), &uuid2ObjDef );
                break;
            case Netn2TransportStruct::E_Disembarkment:
                object.transportData.dataDisembarkment.appointment = tmp.dataDisembarkment.appointment;
                object.transportData.dataDisembarkment.objectToManage.resize( tmp.dataDisembarkment.objectToManage.size() );
                std::transform( tmp.dataDisembarkment.objectToManage.begin(), tmp.dataDisembarkment.objectToManage.end(),
                    object.transportData.dataDisembarkment.objectToManage.begin(), &uuid2ObjDef );
                break;
            case Netn2TransportStruct::E_Embarkment:
                object.transportData.dataEmbarkment.appointment = tmp.dataEmbarkment.appointment;
                object.transportData.dataEmbarkment.objectToManage.resize( tmp.dataEmbarkment.objectToManage.size() );
                std::transform( tmp.dataEmbarkment.objectToManage.begin(), tmp.dataEmbarkment.objectToManage.end(),
                    object.transportData.dataEmbarkment.objectToManage.begin(), &uuid2ObjDef );
                break;
            }
        }
        virtual void Encode( ::hla::Serializer_ABC& serializer, const T& object ) const
        {
            Netn2TransportStruct tmp;
            tmp.transportType = static_cast< Netn2TransportStruct::TransportType >( object.transportData.convoyType );
            switch( tmp.transportType )
            {
            case Netn2TransportStruct::E_Transport:
                tmp.dataTransport.appointment = object.transportData.dataTransport.appointment;
                tmp.dataTransport.finalAppointment = object.transportData.dataTransport.finalAppointment;
                tmp.dataTransport.objectToManage.resize( object.transportData.dataTransport.objectToManage.size() );
                std::transform( object.transportData.dataTransport.objectToManage.begin(), 
                    object.transportData.dataTransport.objectToManage.end(), tmp.dataTransport.objectToManage.begin(),
                    &objectDef2uuid );
                break;
            case Netn2TransportStruct::E_Disembarkment:
                tmp.dataDisembarkment.appointment = object.transportData.dataDisembarkment.appointment;
                tmp.dataDisembarkment.objectToManage.resize( object.transportData.dataDisembarkment.objectToManage.size() );
                std::transform( object.transportData.dataDisembarkment.objectToManage.begin(), 
                    object.transportData.dataDisembarkment.objectToManage.end(), tmp.dataDisembarkment.objectToManage.begin(),
                    &objectDef2uuid );
                break;
            case Netn2TransportStruct::E_Embarkment:
                tmp.dataEmbarkment.appointment = object.transportData.dataEmbarkment.appointment;
                tmp.dataEmbarkment.objectToManage.resize( object.transportData.dataEmbarkment.objectToManage.size() );
                std::transform( object.transportData.dataEmbarkment.objectToManage.begin(), 
                    object.transportData.dataEmbarkment.objectToManage.end(), tmp.dataEmbarkment.objectToManage.begin(),
                    &objectDef2uuid );
                break;
            }
            tmp.Serialize( serializer );
        }
    };

    template< typename T >
    struct ArrayOfUuidDecoder : public ::hla::Encodable_ABC< T >
    {
    private:
    	interactions::ListOfUnits T::* member_;

    public:
    	ArrayOfUuidDecoder(interactions::ListOfUnits T::* m) : member_(m) {}
    	virtual void Decode( ::hla::Deserializer_ABC& deserializer, T& object ) const
    	{
    		VariableArray< NETN_UUID > tmp;
    		tmp.Deserialize(deserializer);
    		(object.*member_).list.resize(tmp.list.size());
    		std::transform( tmp.list.begin(), tmp.list.end(), (object.*member_).list.begin(), &uuid2ObjDef );
    	}
    	virtual void Encode( ::hla::Serializer_ABC& serializer, const T& object ) const
    	{
    		VariableArray< NETN_UUID > tmp;
    		tmp.list.resize((object.*member_).list.size());
    		std::transform( (object.*member_).list.begin(), (object.*member_).list.end(), tmp.list.begin(), &objectDef2uuid );
    		tmp.Serialize(serializer);
    	}
    };

    template< typename T >
    struct TransportUnitIdentifierDecoder : public ::hla::Encodable_ABC< T >
    {
    	virtual void Decode( ::hla::Deserializer_ABC& deserializer, T& object ) const
    	{
    		NETN_UUID tmp;
    		tmp.Deserialize(deserializer);
    		boost::uuids::uuid uid;
    		const std::vector< char > tmpData = tmp.data();
    		memcpy(&uid, tmpData.data(), 16);
    		object.transportUnitIdentifier = UnicodeString(boost::lexical_cast<std::string>(uid));
    	}
    	virtual void Encode( ::hla::Serializer_ABC& serializer, const T& object ) const
    	{
    		boost::uuids::string_generator gen;
    		const boost::uuids::uuid uid = gen(object.transportUnitIdentifier.str());
    		std::vector<char> v(uid.size());
    		std::copy(uid.begin(), uid.end(), v.begin());
    		NETN_UUID tmp(v);
    		tmp.Serialize(serializer);
    	}
    };
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
NETNv2_InteractionBuilder::NETNv2_InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate )
    : logger_ ( logger )
    , federate_ ( federate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder destructor
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
NETNv2_InteractionBuilder::~NETNv2_InteractionBuilder()
{
    // TODO Auto-generated destructor stub
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_RequestService.RequestTransport";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "RequestTimeOut", &interactions::NetnRequestConvoy::requestTimeOut );
    interaction.Register( "TransportData", * new TransportDataEncoder< interactions::NetnRequestConvoy >() );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_OfferService.OfferTransport";
    REGISTER( "IsOffering"        , &interactions::NetnOfferConvoy::isOffering );
    REGISTER( "RequestTimeOut"    , &interactions::NetnOfferConvoy::requestTimeOut );
    RegisterNetnService( interaction, name, logger_ );
    interaction.Register( "TransportData", * new TransportDataEncoder< interactions::NetnOfferConvoy >() );
    REGISTER( "OfferType"         , &interactions::NetnOfferConvoy::offerType );
    interaction.Register( "Transporters", * new ArrayOfUuidDecoder<interactions::NetnOfferConvoy>(&interactions::NetnOfferConvoy::listOfTransporters) );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_AcceptOffer";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_RejectOffer";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::NetnRejectOfferConvoy::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_CancelService";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::NetnCancelConvoy::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_ReadyToReceiveService";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_ServiceStarted";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const
{
    const std::string name = "SCP_Service.TransportEmbarkmentStatus";
    RegisterNetnService( interaction, name, logger_ );
    interaction.Register( "EmbarkedObjects", * new ArrayOfUuidDecoder<interactions::NetnConvoyEmbarkmentStatus>(&interactions::NetnConvoyEmbarkmentStatus::listOfObjectEmbarked) );
    interaction.Register( "TransportUnitIdentifier", * new TransportUnitIdentifierDecoder< interactions::NetnConvoyEmbarkmentStatus >() );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const
{
    const std::string name = "SCP_Service.TransportDisembarkmentStatus";
    RegisterNetnService( interaction, name, logger_ );
    interaction.Register( "DisembarkedObjects", * new ArrayOfUuidDecoder<interactions::NetnConvoyDisembarkmentStatus>(&interactions::NetnConvoyDisembarkmentStatus::listOfObjectDisembarked) );
    interaction.Register( "TransportUnitIdentifier", * new TransportUnitIdentifierDecoder< interactions::NetnConvoyDisembarkmentStatus >() );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const
{
    const std::string name = "SCP_Service.TransportDestroyedEntities";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "DestroyedObjects", &interactions::NetnConvoyDestroyedEntities::listOfEmbarkedObjectDestroyed );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_ServiceComplete";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const
{
    const std::string name = "SCP_Service.SCP_ServiceReceived";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::DoRegister
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::DoRegister( const std::string& name, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) const
{
    try
    {
        federate_.Register( ::hla::InteractionIdentifier( name ), interactionClass, publish, subscribe );
    }
    catch( const ::hla::HLAException& e )
    {
        logger_.LogError("Failed to register interaction "+name+ " : " + tools::GetExceptionMsg( e ) );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& interaction ) const
{
    const std::string name = "TMR.TMR_OfferTransferModellingResponsibility";
    RegisterTMR( interaction, name, logger_ );
    REGISTER( "isOffering", &interactions::TMR_OfferTransferModellingResponsibility::isOffering );
    REGISTER( "Reason", &interactions::TMR_OfferTransferModellingResponsibility::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& interaction ) const
{
    const std::string name = "TMR.TMR_InitiateTransferModellingResponsibility";
    RegisterTMR( interaction, name, logger_ );
    REGISTER( "TransferType", &interactions::TMR_InitiateTransferModellingResponsibility::transferType );
    REGISTER( "Instances", &interactions::TMR_InitiateTransferModellingResponsibility::instances );
    REGISTER( "Attributes", &interactions::TMR_InitiateTransferModellingResponsibility::attributes );
    REGISTER( "Initiating", &interactions::TMR_InitiateTransferModellingResponsibility::intiating );
    REGISTER( "CapabilityType", &interactions::TMR_InitiateTransferModellingResponsibility::capabilityType );
    REGISTER( "InstanceAttributeValues", &interactions::TMR_InitiateTransferModellingResponsibility::attributeValues );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& interaction ) const
{
    const std::string name = "TMR.TMR_RequestTransferModellingResponsibility";
    RegisterTMR( interaction, name, logger_ );
    REGISTER( "TransferType", &interactions::TMR_RequestTransferModellingResponsibility::transferType );
    REGISTER( "Instances", &interactions::TMR_RequestTransferModellingResponsibility::instances );
    REGISTER( "Attributes", &interactions::TMR_RequestTransferModellingResponsibility::attributes );
    REGISTER( "CapabilityType", &interactions::TMR_RequestTransferModellingResponsibility::capabilityType );
    REGISTER( "InstanceAttributeValues", &interactions::TMR_RequestTransferModellingResponsibility::attributeValues );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-07-02
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_CancelRequest >& interaction ) const
{
    const std::string name = "TMR.TMR_CancelRequest";
    RegisterTMR( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::TMR_CancelRequest::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: NETNv2_InteractionBuilder::Build
// Created: AHC 2012-07-02
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_TransferResult >& interaction ) const
{
    const std::string name = "TMR.TMR_TransferResult";
    RegisterTMR( interaction, name, logger_ );
    REGISTER( "TransferOk", &interactions::TMR_TransferResult::transferOk );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_DisaggregationRequest >& interaction ) const
{
    const std::string name = "MRM_Object.MRM_DisaggregationRequest";
    RegisterMRM( interaction, name, logger_ );
    REGISTER( "AggregateUUID", &interactions::MRM_DisaggregationRequest::aggregateUUID );
    REGISTER( "AggregationState", &interactions::MRM_DisaggregationRequest::aggregationState );
    REGISTER( "UuidList", &interactions::MRM_DisaggregationRequest::uuidsList );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_DisaggregationResponse >& interaction ) const
{
    const std::string name = "MRM_Object.MRM_DisaggregationResponse";
    RegisterMRM( interaction, name, logger_ );
    REGISTER( "Acknowledge", &interactions::MRM_DisaggregationResponse::acknowledge );
    REGISTER( "NonComplianceReason", &interactions::MRM_DisaggregationResponse::nonComplianceReason );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_AggregationRequest >& interaction ) const
{
    const std::string name = "MRM_Object.MRM_AggregationRequest";
    RegisterMRM( interaction, name, logger_ );
    REGISTER( "UuidList", &interactions::MRM_AggregationRequest::uuidsList );
    REGISTER( "AggregateUuid", &interactions::MRM_AggregationRequest::aggregateUUID );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_AggregationResponse >& interaction ) const
{
    const std::string name = "MRM_Object.MRM_AggregationResponse";
    RegisterMRM( interaction, name, logger_ );
    REGISTER( "Acknowledge", &interactions::MRM_AggregationResponse::acknowledge );
    REGISTER( "NonComplianceReason", &interactions::MRM_AggregationResponse::nonComplianceReason );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_CancelRequest >& interaction ) const
{
    const std::string name = "MRM_Object.MRM_CancelRequest";
    RegisterMRM( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::MRM_CancelRequest::reason );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_ActionComplete >& interaction ) const
{
    const std::string name = "MRM_Object.MRM_ActionComplete";
    RegisterMRM( interaction, name, logger_ );
    REGISTER( "CompletionResult", &interactions::MRM_ActionComplete::result );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_Trigger >& interaction ) const
{
    const std::string name = "MRM_Trigger";
    REGISTER( "Instance", &interactions::MRM_Trigger::instance );
    REGISTER( "AggregationState", &interactions::MRM_Trigger::aggregationState );
    REGISTER( "UuidList", &interactions::MRM_Trigger::uuidList );
    return DoRegister( name, interaction, true, true );
}

bool NETNv2_InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_TriggerResponse >& interaction ) const
{
    const std::string name = "MRM_TriggerResponse";
    REGISTER( "Instance", &interactions::MRM_TriggerResponse::instance );
    REGISTER( "TransactionID", &interactions::MRM_TriggerResponse::transactionID );
    return DoRegister( name, interaction, true, true );
}
