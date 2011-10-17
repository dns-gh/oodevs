// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnServiceComplete.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnServiceComplete constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceComplete::NetnServiceComplete( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnServiceComplete >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnServiceComplete >( notification ) )
{
    pInteraction_->Register( "ServiceID"  , ::hla::CreateParameter( &interactions::NetnServiceComplete::serviceId ) );
    pInteraction_->Register( "Consumer"   , ::hla::CreateParameter( &interactions::NetnServiceComplete::consumer ) );
    pInteraction_->Register( "Provider"   , ::hla::CreateParameter( &interactions::NetnServiceComplete::provider ) );
    pInteraction_->Register( "ServiceType", ::hla::CreateParameter( &interactions::NetnServiceComplete::serviceType ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceComplete" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnServiceComplete destructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceComplete::~NetnServiceComplete()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceComplete::Send
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnServiceComplete::Send( const interactions::NetnServiceComplete& interaction )
{
    pInteraction_->Send( interaction );
}
