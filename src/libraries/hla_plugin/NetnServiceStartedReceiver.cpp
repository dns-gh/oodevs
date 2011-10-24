// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnServiceStartedReceiver.h"
#include "TransportationRequester_ABC.h"
#include "Interactions.h"
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnServiceStartedReceiver constructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceStartedReceiver::NetnServiceStartedReceiver( TransportationRequester_ABC& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceStartedReceiver destructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnServiceStartedReceiver::~NetnServiceStartedReceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnServiceStartedReceiver::Receive
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnServiceStartedReceiver::Receive( interactions::NetnServiceStarted& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    controller_.ServiceStarted( interaction.serviceId.eventCount );
}
