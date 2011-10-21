// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnReadyToReceiveService.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnReadyToReceiveService constructor
// Created: VPR 2011-10-11
// -----------------------------------------------------------------------------
NetnReadyToReceiveService::NetnReadyToReceiveService( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnReadyToReceiveService >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnReadyToReceiveService >( notification ) )
{
    RegisterNetnService( *pInteraction_ );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ReadyToReceiveService" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnReadyToReceiveService destructor
// Created: VPR 2011-10-11
// -----------------------------------------------------------------------------
NetnReadyToReceiveService::~NetnReadyToReceiveService()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnReadyToReceiveService::Send
// Created: VPR 2011-10-11
// -----------------------------------------------------------------------------
void NetnReadyToReceiveService::Send( const interactions::NetnReadyToReceiveService& interaction )
{
    pInteraction_->Send( interaction );
}
