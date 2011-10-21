// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnCancelConvoy.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "NetnService.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnCancelConvoy constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnCancelConvoy::NetnCancelConvoy( Federate_ABC& federate )
    : pInteraction_( new ::hla::Interaction< interactions::NetnCancelConvoy >( *this ) )
{
    RegisterNetnService( *pInteraction_ );
    pInteraction_->Register( "Reason"     , ::hla::CreateParameter( &interactions::NetnCancelConvoy::reason ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_CancelService.NETN_CancelConvoy" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnCancelConvoy destructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnCancelConvoy::~NetnCancelConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnCancelConvoy::Send
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnCancelConvoy::Send( const interactions::NetnCancelConvoy& interaction )
{
    pInteraction_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: NetnCancelConvoy::Receive
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnCancelConvoy::Receive( interactions::NetnCancelConvoy& /*interaction*/ )
{
    // NOTHING
}
