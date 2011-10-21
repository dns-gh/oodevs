// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnServiceStarted.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnServiceStarted constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceStarted::NetnServiceStarted( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnServiceStarted >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnServiceStarted >( notification ) )
{
    RegisterNetnService( *pInteraction_ );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceStarted" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnServiceStarted destructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceStarted::~NetnServiceStarted()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceStarted::Send
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnServiceStarted::Send( const interactions::NetnServiceStarted& interaction )
{
    pInteraction_->Send( interaction );
}
