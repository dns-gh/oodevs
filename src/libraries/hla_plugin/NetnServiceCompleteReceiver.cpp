// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnServiceCompleteReceiver.h"
#include "TransportationRequester_ABC.h"
#include "Interactions.h"
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnServiceCompleteReceiver constructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceCompleteReceiver::NetnServiceCompleteReceiver( TransportationRequester_ABC& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceCompleteReceiver destructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceCompleteReceiver::~NetnServiceCompleteReceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceCompleteReceiver::Receive
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnServiceCompleteReceiver::Receive( interactions::NetnServiceComplete& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    controller_.ServiceComplete( interaction.serviceId.eventCount, interaction.provider.str() );
}
