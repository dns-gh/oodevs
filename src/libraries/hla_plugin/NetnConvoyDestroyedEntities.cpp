// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnConvoyDestroyedEntities.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnConvoyDestroyedEntities constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnConvoyDestroyedEntities::NetnConvoyDestroyedEntities( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnConvoyDestroyedEntities >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >( notification ) )
{
    RegisterNetnService( *pInteraction_ );
    pInteraction_->Register( "ListOfEmbarkedObjectDestroyed", ::hla::CreateParameter( &interactions::NetnConvoyDestroyedEntities::listOfEmbarkedObjectDestroyed ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ConvoyDestroyedEntities" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyDestroyedEntities destructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnConvoyDestroyedEntities::~NetnConvoyDestroyedEntities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyDestroyedEntities::Send
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnConvoyDestroyedEntities::Send( const interactions::NetnConvoyDestroyedEntities& interaction )
{
    pInteraction_->Send( interaction );
}
