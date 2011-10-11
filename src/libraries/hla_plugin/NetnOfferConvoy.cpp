// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnOfferConvoy.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "SerializationTools.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoy constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnOfferConvoy::NetnOfferConvoy( Federate_ABC& federate )
    : pInteraction_( new ::hla::Interaction< interactions::NetnOfferConvoy >( *this ) )
{
    pInteraction_->Register( "IsOffering"        , ::hla::CreateParameter( &interactions::NetnOfferConvoy::isOffering ) );
    pInteraction_->Register( "RequestTimeOut"    , ::hla::CreateParameter( &interactions::NetnOfferConvoy::requestTimeOut ) );
    pInteraction_->Register( "ServiceID"         , ::hla::CreateParameter( &interactions::NetnOfferConvoy::serviceId ) );
    pInteraction_->Register( "Consumer"          , ::hla::CreateParameter( &interactions::NetnOfferConvoy::consumer ) );
    pInteraction_->Register( "Provider"          , ::hla::CreateParameter( &interactions::NetnOfferConvoy::provider ) );
    pInteraction_->Register( "ServiceType"       , ::hla::CreateParameter( &interactions::NetnOfferConvoy::serviceType ) );
    pInteraction_->Register( "TransportData"     , ::hla::CreateParameter( &interactions::NetnOfferConvoy::transportData ) );
    pInteraction_->Register( "OfferType"         , ::hla::CreateParameter( &interactions::NetnOfferConvoy::offerType ) );
    pInteraction_->Register( "ListOfTransporters", ::hla::CreateParameter( &interactions::NetnOfferConvoy::listOfTransporters ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_OfferService.NETN_OfferConvoy" ), *pInteraction_, true, false );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoy destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnOfferConvoy::~NetnOfferConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoy::Send
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void NetnOfferConvoy::Send( const interactions::NetnOfferConvoy& interaction )
{
    pInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoy::Receive
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void NetnOfferConvoy::Receive( interactions::NetnOfferConvoy& /*interaction*/ )
{
    throw std::runtime_error( __FUNCTION__ " unimplemented function" );
}
