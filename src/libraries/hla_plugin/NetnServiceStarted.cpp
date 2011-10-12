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
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnServiceStarted constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceStarted::NetnServiceStarted( Federate_ABC& federate )
    : pInteraction_( new ::hla::Interaction< interactions::NetnServiceStarted >( *this ) )
{
    pInteraction_->Register( "ServiceID"  , ::hla::CreateParameter( &interactions::NetnServiceStarted::serviceId ) );
    pInteraction_->Register( "Consumer"   , ::hla::CreateParameter( &interactions::NetnServiceStarted::consumer ) );
    pInteraction_->Register( "Provider"   , ::hla::CreateParameter( &interactions::NetnServiceStarted::provider ) );
    pInteraction_->Register( "ServiceType", ::hla::CreateParameter( &interactions::NetnServiceStarted::serviceType ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ServiceStarted" ), *pInteraction_, true, false );
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

// -----------------------------------------------------------------------------
// Name: NetnServiceStarted::Receive
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnServiceStarted::Receive( interactions::NetnServiceStarted& interaction )
{
    throw std::runtime_error( __FUNCTION__ " unimplemented function" );
}
