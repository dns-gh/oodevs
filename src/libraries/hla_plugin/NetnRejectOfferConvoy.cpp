// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRejectOfferConvoy.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "SerializationTools.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRejectOfferConvoy constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnRejectOfferConvoy::NetnRejectOfferConvoy( Federate_ABC& federate )
    : pInteraction_( new ::hla::Interaction< interactions::NetnRejectOfferConvoy >( *this ) )
{
    RegisterNetnService( *pInteraction_ );
    pInteraction_->Register( "Reason"     , ::hla::CreateParameter( &interactions::NetnRejectOfferConvoy::reason ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_RejectOffer.NETN_RejectOfferConvoy" ), *pInteraction_, true, false );
}

// -----------------------------------------------------------------------------
// Name: NetnRejectOfferConvoy destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnRejectOfferConvoy::~NetnRejectOfferConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRejectOfferConvoy::Send
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void NetnRejectOfferConvoy::Send( const interactions::NetnRejectOfferConvoy& interaction )
{
    pInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: NetnRejectOfferConvoy::Receive
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void NetnRejectOfferConvoy::Receive( interactions::NetnRejectOfferConvoy& /*interaction*/ )
{
    throw std::runtime_error( __FUNCTION__ " unimplemented function" );
}
