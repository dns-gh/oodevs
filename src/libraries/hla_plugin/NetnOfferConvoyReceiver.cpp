// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnOfferConvoyReceiver.h"
#include "TransportationRequester_ABC.h"
#include "Interactions.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoyReceiver constructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnOfferConvoyReceiver::NetnOfferConvoyReceiver( TransportationRequester_ABC& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoyReceiver destructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnOfferConvoyReceiver::~NetnOfferConvoyReceiver()
{
    // NOTHING
}

namespace
{
    bool CheckOffer( const interactions::NetnOfferConvoy& offer )
    {
        return offer.offerType == 1;
    }
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoyReceiver::Receive
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferConvoyReceiver::Receive( interactions::NetnOfferConvoy& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.transportData.convoyType != 0 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    if( !interaction.isOffering )
        return;
    controller_.OfferReceived( interaction.serviceId.eventCount, CheckOffer( interaction ), interaction.provider.str(), interaction.listOfTransporters );
}
