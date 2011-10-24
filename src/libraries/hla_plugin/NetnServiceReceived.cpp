// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnServiceReceived.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnServiceReceived constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceReceived::NetnServiceReceived( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnServiceReceived >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnServiceReceived >( notification ) )
{
    RegisterNetnService( *pInteraction_ );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceReceived" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnServiceReceived destructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceReceived::~NetnServiceReceived()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceReceived::Send
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnServiceReceived::Send( const interactions::NetnServiceReceived& interaction )
{
    pInteraction_->Send( interaction );
}
