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
#include "NETN_InteractionBuilder_ABC.h"
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

}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder constructor
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
InteractionBuilder::InteractionBuilder( dispatcher::Logger_ABC& logger, Federate_ABC& federate, const NETN_InteractionBuilder_ABC& netnBuilder )
    : logger_  ( logger )
    , federate_( federate )
    , netnBuilder_ ( netnBuilder )
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
    catch( const ::hla::HLAException& e )
    {
        logger_.LogError("Failed to register interaction "+name+ " : " + tools::GetExceptionMsg( e ) );
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
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const
{
    return netnBuilder_.Build( interaction );
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

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_OfferTransferModellingResponsibility >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_InitiateTransferModellingResponsibility >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_RequestTransferModellingResponsibility >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2013-07-02
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_CancelRequest >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2013-07-02
// -----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::TMR_TransferResult >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}
// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: AHC 2012-11-15
// ----------------------------------------------------------------------------
bool InteractionBuilder::Build( ::hla::Interaction< interactions::WeaponFire >& interaction ) const
{
    const std::string name = "WeaponFire";
    REGISTER( "EventIdentifier"       , &interactions::WeaponFire::eventIdentifier );
    REGISTER( "FireControlSolutionRange"       , &interactions::WeaponFire::fireControlSolutionRange );
    REGISTER( "FireMissionIndex"       , &interactions::WeaponFire::fireMissionIndex );
    REGISTER( "FiringLocation"       , &interactions::WeaponFire::firingLocation );
    REGISTER( "FiringObjectIdentifier"       , &interactions::WeaponFire::firingObjectIdentifier );
    REGISTER( "FuseType"       , &interactions::WeaponFire::fuseType );
    REGISTER( "InitialVelocityVector"       , &interactions::WeaponFire::initialVelocityVector );
    REGISTER( "MunitionObjectIdentifier"       , &interactions::WeaponFire::munitionObjectIdentifier );
    REGISTER( "MunitionType"       , &interactions::WeaponFire::munitionType );
    REGISTER( "QuantityFired"       , &interactions::WeaponFire::quantityFired );
    REGISTER( "RateOfFire"       , &interactions::WeaponFire::rateOfFire );
    REGISTER( "TargetObjectIdentifier"       , &interactions::WeaponFire::targetObjectIdentifier );
    REGISTER( "WarheadType"       , &interactions::WeaponFire::warheadType );
    return DoRegister( name, interaction, true, true );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_DisaggregationRequest >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_DisaggregationResponse >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_AggregationRequest >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_AggregationResponse >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_CancelRequest >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_ActionComplete >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_Trigger >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}

bool InteractionBuilder::Build( ::hla::Interaction< interactions::MRM_TriggerResponse >& interaction ) const
{
    return netnBuilder_.Build( interaction );
}
