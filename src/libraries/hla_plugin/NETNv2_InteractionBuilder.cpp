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
    NETN_UUID objectDef2uuid( const NetnObjectDefinitionStruct& v )
    {
        return NETN_UUID( v.uniqueId.str() ) ;
    }
    NetnObjectDefinitionStruct uuid2ObjDef( const NETN_UUID& v )
    {
        NetnObjectDefinitionStruct retval;
        retval.uniqueId = UniqueId( v.str() );
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
    REGISTER( "Transporters", &interactions::NetnOfferConvoy::listOfTransporters );
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
    const std::string name = "SCP_Service.SCP_RejectOffer.RejectOfferTransport";
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
    const std::string name = "SCP_Service.SCP_CancelService.CancelTransport";
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
    REGISTER( "EmbarkedObjects"   , &interactions::NetnConvoyEmbarkmentStatus::listOfObjectEmbarked );
    REGISTER( "TransportUnitIdentifier", &interactions::NetnConvoyEmbarkmentStatus::transportUnitIdentifier );
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
    REGISTER( "DisembarkedObjects", &interactions::NetnConvoyDisembarkmentStatus::listOfObjectDisembarked );
    REGISTER( "TransportUnitIdentifier", &interactions::NetnConvoyDisembarkmentStatus::transportUnitIdentifier );
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
// Name: InteractionBuilder::DoRegister
// Created: AHC 2012-08-31
// -----------------------------------------------------------------------------
bool NETNv2_InteractionBuilder::DoRegister( const std::string& name, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) const
{
    try
    {
        federate_.Register( ::hla::InteractionIdentifier( name ), interactionClass, publish, subscribe );
    }
    catch (const ::hla::HLAException& ex)
    {
        logger_.LogError("Failed to register interaction "+name+ " : " + ex.what() );
        return false;
    }
    return true;
}
