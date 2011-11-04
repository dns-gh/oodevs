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
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

namespace
{
    template< typename T >
    void RegisterNetnService( ::hla::Interaction< T >& interaction )
    {
        interaction.Register( "ServiceID"  , ::hla::CreateParameter< T >( &T::serviceId ) );
        interaction.Register( "Consumer"   , ::hla::CreateParameter< T >( &T::consumer ) );
        interaction.Register( "Provider"   , ::hla::CreateParameter< T >( &T::provider ) );
        interaction.Register( "ServiceType", ::hla::CreateParameter< T >( &T::serviceType ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder constructor
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
InteractionBuilder::InteractionBuilder()
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
// Name: InteractionBuilder::Build
// Created: SLI 2011-11-04
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::MunitionDetonation >& interaction ) const
{
    interaction.Register( "ArticulatedPartData"       , ::hla::CreateParameter( &interactions::MunitionDetonation::articulatedPartData ) );
    interaction.Register( "DetonationLocation"        , ::hla::CreateParameter( &interactions::MunitionDetonation::detonationLocation ) );
    interaction.Register( "DetonationResultCode"      , ::hla::CreateParameter( &interactions::MunitionDetonation::detonationResultCode ) );
    interaction.Register( "EventIdentifier"           , ::hla::CreateParameter( &interactions::MunitionDetonation::eventIdentifier ) );
    interaction.Register( "FiringObjectIdentifier"    , ::hla::CreateParameter( &interactions::MunitionDetonation::firingObjectIdentifier ) );
    interaction.Register( "FinalVelocityVector"       , ::hla::CreateParameter( &interactions::MunitionDetonation::finalVelocityVector ) );
    interaction.Register( "FuseType"                  , ::hla::CreateParameter( &interactions::MunitionDetonation::fuseType ) );
    interaction.Register( "MunitionObjectIdentifier"  , ::hla::CreateParameter( &interactions::MunitionDetonation::munitionObjectIdentifier ) );
    interaction.Register( "MunitionType"              , ::hla::CreateParameter( &interactions::MunitionDetonation::munitionType ) );
    interaction.Register( "QuantityFired"             , ::hla::CreateParameter( &interactions::MunitionDetonation::quantityFired ) );
    interaction.Register( "RateOfFire"                , ::hla::CreateParameter( &interactions::MunitionDetonation::rateOfFire ) );
    interaction.Register( "RelativeDetonationLocation", ::hla::CreateParameter( &interactions::MunitionDetonation::relativeDetonationLocation ) );
    interaction.Register( "TargetObjectIdentifier"    , ::hla::CreateParameter( &interactions::MunitionDetonation::targetObjectIdentifier ) );
    interaction.Register( "WarheadType"               , ::hla::CreateParameter( &interactions::MunitionDetonation::warheadType ) );
    federate.Register( ::hla::InteractionIdentifier( "MunitionDetonation" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnRequestConvoy >& interaction ) const
{
    RegisterNetnService( interaction );
    interaction.Register( "RequestTimeOut", ::hla::CreateParameter( &interactions::NetnRequestConvoy::requestTimeOut ) );
    interaction.Register( "TransportData" , ::hla::CreateParameter( &interactions::NetnRequestConvoy::transportData ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_RequestService.NETN_RequestConvoy" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnOfferConvoy >& interaction ) const
{
    interaction.Register( "IsOffering"        , ::hla::CreateParameter( &interactions::NetnOfferConvoy::isOffering ) );
    interaction.Register( "RequestTimeOut"    , ::hla::CreateParameter( &interactions::NetnOfferConvoy::requestTimeOut ) );
    RegisterNetnService( interaction );
    interaction.Register( "TransportData"     , ::hla::CreateParameter( &interactions::NetnOfferConvoy::transportData ) );
    interaction.Register( "OfferType"         , ::hla::CreateParameter( &interactions::NetnOfferConvoy::offerType ) );
    interaction.Register( "ListOfTransporters", ::hla::CreateParameter( &interactions::NetnOfferConvoy::listOfTransporters ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_OfferService.NETN_OfferConvoy" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnAcceptOffer >& interaction ) const
{
    RegisterNetnService( interaction );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_AcceptOffer" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnRejectOfferConvoy >& interaction ) const
{
    RegisterNetnService( interaction );
    interaction.Register( "Reason", ::hla::CreateParameter( &interactions::NetnRejectOfferConvoy::reason ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnCancelConvoy >& interaction ) const
{
    RegisterNetnService( interaction );
    interaction.Register( "Reason", ::hla::CreateParameter( &interactions::NetnCancelConvoy::reason ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_CancelService.NETN_CancelConvoy" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnReadyToReceiveService >& interaction ) const
{
    RegisterNetnService( interaction );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ReadyToReceiveService" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnServiceStarted >& interaction ) const
{
    RegisterNetnService( interaction );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceStarted" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >& interaction ) const
{
    RegisterNetnService( interaction );
    interaction.Register( "ListOfObjectEmbarked"   , ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::listOfObjectEmbarked ) );
    interaction.Register( "TransportUnitIdentifier", ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::transportUnitIdentifier ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ConvoyEmbarkmentStatus" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >& interaction ) const
{
    RegisterNetnService( interaction );
    interaction.Register( "ListOfObjectDisembarked", ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::listOfObjectDisembarked ) );
    interaction.Register( "TransportUnitIdentifier", ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::transportUnitIdentifier ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ConvoyDisembarkmentStatus" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >& interaction ) const
{
    RegisterNetnService( interaction );
    interaction.Register( "ListOfEmbarkedObjectDestroyed", ::hla::CreateParameter( &interactions::NetnConvoyDestroyedEntities::listOfEmbarkedObjectDestroyed ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ConvoyDestroyedEntities" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnServiceComplete >& interaction ) const
{
    RegisterNetnService( interaction );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceComplete" ), interaction, true, true );
}

// -----------------------------------------------------------------------------
// Name: InteractionBuilder::Build
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void InteractionBuilder::Build( Federate_ABC& federate, ::hla::Interaction< interactions::NetnServiceReceived >& interaction ) const
{
    RegisterNetnService( interaction );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceReceived" ), interaction, true, true );
}
