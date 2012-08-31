// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "InteractionBuilder.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "SerializationTools.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

namespace
{
    template< typename T, typename R >
    class Encodable : public ::hla::Encodable< T, R >
    {
    public:
        Encodable( R T::* pMemberData, const std::string& interaction, const std::string& parameter, dispatcher::Logger_ABC& logger )
            : ::hla::Encodable< T, R >( pMemberData )
            , interaction_( interaction )
            , parameter_  ( parameter )
            , logger_     ( logger )
        {
            // NOTHING
        }
        virtual void Encode( ::hla::Serializer_ABC& serializer, const T& object ) const
        {
            ::hla::Encodable< T, R >::Encode( serializer, object );
        }
        virtual void Decode( ::hla::Deserializer_ABC& deserializer, T& object ) const
        {
            try
            {
                ::hla::Encodable< T, R >::Decode( deserializer, object );
            }
            catch( const std::exception& e )
            {
                logger_.LogError( "Interaction '" + interaction_ + "' parameter '" + parameter_ + "' : " + e.what() );
            }
        }
    private:
        const std::string interaction_;
        const std::string parameter_;
        dispatcher::Logger_ABC& logger_;
    };
    template< typename T, typename R, typename Interaction >
    void Register( ::hla::Interaction< Interaction >& interaction, const std::string& name, const std::string& parameter, R T::* pMemberData, dispatcher::Logger_ABC& logger )
    {
        interaction.Register( parameter, *new ::Encodable< T, R >( pMemberData, name, parameter, logger ) );
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
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder constructor
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
InteractionBuilder::InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate )
    : logger_  ( logger )
    , federate_( federate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder destructor
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
InteractionBuilder::~InteractionBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::DoRegister
// Created: AHC 2012-07-04
// -----------------------------------------------------------------------------
bool InteractionBuilder::DoRegister( const std::string& name, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe ) const
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

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-11-04
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::MunitionDetonation >& interaction ) const
{
    const std::string name = "MunitionDetonation";
    REGISTER( "ArticulatedPartData"       , &interactions::MunitionDetonation::articulatedPartData );
    REGISTER( "DetonationLocation"        , &interactions::MunitionDetonation::detonationLocation );
    REGISTER( "DetonationResultCode"      , &interactions::MunitionDetonation::detonationResultCode );
    REGISTER( "EventIdentifier"           , &interactions::MunitionDetonation::eventIdentifier );
    REGISTER( "FiringObjectIdentifier"    , &interactions::MunitionDetonation::firingObjectIdentifier );
    REGISTER( "FinalVelocityVector"       , &interactions::MunitionDetonation::finalVelocityVector );
    REGISTER( "FuseType"                  , &interactions::MunitionDetonation::fuseType );
    REGISTER( "MunitionObjectIdentifier"  , &interactions::MunitionDetonation::munitionObjectIdentifier );
    REGISTER( "MunitionType"              , &interactions::MunitionDetonation::munitionType );
    REGISTER( "QuantityFired"             , &interactions::MunitionDetonation::quantityFired );
    REGISTER( "RateOfFire"                , &interactions::MunitionDetonation::rateOfFire );
    REGISTER( "RelativeDetonationLocation", &interactions::MunitionDetonation::relativeDetonationLocation );
    REGISTER( "TargetObjectIdentifier"    , &interactions::MunitionDetonation::targetObjectIdentifier );
    REGISTER( "WarheadType"               , &interactions::MunitionDetonation::warheadType );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_RequestService.NETN_RequestConvoy";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "RequestTimeOut", &interactions::NetnRequestConvoy::requestTimeOut );
    REGISTER( "TransportData" , &interactions::NetnRequestConvoy::transportData );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_OfferService.NETN_OfferConvoy";
    REGISTER( "IsOffering"        , &interactions::NetnOfferConvoy::isOffering );
    REGISTER( "RequestTimeOut"    , &interactions::NetnOfferConvoy::requestTimeOut );
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "TransportData"     , &interactions::NetnOfferConvoy::transportData );
    REGISTER( "OfferType"         , &interactions::NetnOfferConvoy::offerType );
    REGISTER( "ListOfTransporters", &interactions::NetnOfferConvoy::listOfTransporters );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_AcceptOffer";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::NetnRejectOfferConvoy::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_CancelService.NETN_CancelConvoy";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "Reason", &interactions::NetnCancelConvoy::reason );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ReadyToReceiveService";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ServiceStarted";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ConvoyEmbarkmentStatus";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "ListOfObjectEmbarked"   , &interactions::NetnConvoyEmbarkmentStatus::listOfObjectEmbarked );
    REGISTER( "TransportUnitIdentifier", &interactions::NetnConvoyEmbarkmentStatus::transportUnitIdentifier );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ConvoyDisembarkmentStatus";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "ListOfObjectDisembarked", &interactions::NetnConvoyDisembarkmentStatus::listOfObjectDisembarked );
    REGISTER( "TransportUnitIdentifier", &interactions::NetnConvoyDisembarkmentStatus::transportUnitIdentifier );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ConvoyDestroyedEntities";
    RegisterNetnService( interaction, name, logger_ );
    REGISTER( "ListOfEmbarkedObjectDestroyed", &interactions::NetnConvoyDestroyedEntities::listOfEmbarkedObjectDestroyed );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ServiceComplete";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const
{
    const std::string name = "NETN_Service.NETN_ServiceReceived";
    RegisterNetnService( interaction, name, logger_ );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-02-06
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::ResupplyCancel >& interaction ) const
{
    const std::string name = "ResupplyCancel";
    REGISTER( "ReceivingObject"           , &interactions::ResupplyCancel::receivingObject );
    REGISTER( "SupplyingObject"           , &interactions::ResupplyCancel::supplyingObject );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-02-06
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::ResupplyOffer >& interaction ) const
{
    const std::string name = "ResupplyOffer";
    REGISTER( "ReceivingObject"           , &interactions::ResupplyOffer::receivingObject );
    REGISTER( "SupplyingObject"           , &interactions::ResupplyOffer::supplyingObject );
    REGISTER( "SuppliesData"              , &interactions::ResupplyOffer::suppliesData );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-02-06
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::ResupplyReceived >& interaction ) const
{
    const std::string name = "ResupplyReceived";
    REGISTER( "ReceivingObject"           , &interactions::ResupplyReceived::receivingObject );
    REGISTER( "SupplyingObject"           , &interactions::ResupplyReceived::supplyingObject );
    REGISTER( "SuppliesData"              , &interactions::ResupplyReceived::suppliesData );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-02-06
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::ServiceRequest >& interaction ) const
{
    const std::string name = "ServiceRequest";
    REGISTER( "RequestingObject"           , &interactions::ServiceRequest::requestingObject );
    REGISTER( "ServicingObject"           , &interactions::ServiceRequest::servicingObject );
    REGISTER( "ServiceType"               , &interactions::ServiceRequest::serviceType );
    REGISTER( "SuppliesData"              , &interactions::ServiceRequest::suppliesData );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-02-07
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::TransferControl >& interaction ) const
{
    const std::string name = "TransferControl";
    REGISTER( "OriginatingEntity"         , &interactions::TransferControl::originatingEntity );
    REGISTER( "ReceivingEntity"           , &interactions::TransferControl::receivingEntity );
    REGISTER( "RequestIdentifier"         , &interactions::TransferControl::requestIdentifier );
    REGISTER( "TransferType"              , &interactions::TransferControl::transferType );
    REGISTER( "TransferEntity"            , &interactions::TransferControl::transferEntity );
    REGISTER( "RecordSetData"             , &interactions::TransferControl::recordSetData );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-02-08
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::Acknowledge >& interaction ) const
{
    const std::string name = "Acknowledge";
    REGISTER( "OriginatingEntity"         , &interactions::Acknowledge::originatingEntity );
    REGISTER( "ReceivingEntity"           , &interactions::Acknowledge::receivingEntity );
    REGISTER( "RequestIdentifier"         , &interactions::Acknowledge::requestIdentifier );
    REGISTER( "AcknowledgeFlag"           , &interactions::Acknowledge::acknowledgeFlag );
    REGISTER( "ResponseFlag"            , &interactions::Acknowledge::responseFlag );
    return DoRegister( name, interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::Comment >& interaction ) const
{
    const std::string name = "Comment";
    REGISTER( "OriginatingEntity"         , &interactions::Comment::originatingEntity );
    REGISTER( "ReceivingEntity"           , &interactions::Comment::receivingEntity );
    REGISTER( "VariableDatumSet"         , &interactions::Comment::variableDatumSet );
    return DoRegister( name, interaction, true, true );
}

