// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnConvoyDisembarkmentStatus.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnConvoyDisembarkmentStatus constructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnConvoyDisembarkmentStatus::NetnConvoyDisembarkmentStatus( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnConvoyDisembarkmentStatus >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >( notification ) )
{
    pInteraction_->Register( "ServiceID"              , ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::serviceId ) );
    pInteraction_->Register( "Consumer"               , ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::consumer ) );
    pInteraction_->Register( "Provider"               , ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::provider ) );
    pInteraction_->Register( "ServiceType"            , ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::serviceType ) );
    pInteraction_->Register( "ListOfObjectDisembarked", ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::listOfObjectDisembarked ) );
    pInteraction_->Register( "TransportUnitIdentifier", ::hla::CreateParameter( &interactions::NetnConvoyDisembarkmentStatus::transportUnitIdentifier ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ConvoyDisembarkmentStatus" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyDisembarkmentStatus destructor
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
NetnConvoyDisembarkmentStatus::~NetnConvoyDisembarkmentStatus()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyDisembarkmentStatus::Send
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnConvoyDisembarkmentStatus::Send( const interactions::NetnConvoyDisembarkmentStatus& interaction )
{
    pInteraction_->Send( interaction );
}
