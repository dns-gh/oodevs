// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnAcceptOffer.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnAcceptOffer constructor
// Created: VPR 2011-10-11
// -----------------------------------------------------------------------------
NetnAcceptOffer::NetnAcceptOffer( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnAcceptOffer >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnAcceptOffer >( notification ) )
{
    RegisterNetnService( *pInteraction_ );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_AcceptOffer" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnAcceptOffer destructor
// Created: VPR 2011-10-11
// -----------------------------------------------------------------------------
NetnAcceptOffer::~NetnAcceptOffer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAcceptOffer::Send
// Created: VPR 2011-10-11
// -----------------------------------------------------------------------------
void NetnAcceptOffer::Send( const interactions::NetnAcceptOffer& interaction )
{
    pInteraction_->Send( interaction );
}
