// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnConvoyEmbarkmentStatus.h"
#include "Federate_ABC.h"
#include "Interactions.h"
#include "SerializationTools.h"
#include <cassert>
#include <hla/Interaction.h>
#include <hla/InteractionIdentifier.h>
#include <hla/Parameter.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnConvoyEmbarkmentStatus constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnConvoyEmbarkmentStatus::NetnConvoyEmbarkmentStatus( Federate_ABC& federate, ::hla::InteractionNotification_ABC< interactions::NetnConvoyEmbarkmentStatus >& notification )
    : pInteraction_( new ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >( notification ) )
{
    pInteraction_->Register( "ServiceID"              , ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::serviceId ) );
    pInteraction_->Register( "Consumer"               , ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::consumer ) );
    pInteraction_->Register( "Provider"               , ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::provider ) );
    pInteraction_->Register( "ServiceType"            , ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::serviceType ) );
    pInteraction_->Register( "ListOfObjectEmbarked"   , ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::listOfObjectEmbarked ) );
    pInteraction_->Register( "TransportUnitIdentifier", ::hla::CreateParameter( &interactions::NetnConvoyEmbarkmentStatus::transportUnitIdentifier ) );
    federate.Register( ::hla::InteractionIdentifier( "NETN_Service.NETN_ConvoyEmbarkmentStatus" ), *pInteraction_, true, true );
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyEmbarkmentStatus destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnConvoyEmbarkmentStatus::~NetnConvoyEmbarkmentStatus()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyEmbarkmentStatus::Send
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void NetnConvoyEmbarkmentStatus::Send( const interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    pInteraction_->Send( interaction );
}
