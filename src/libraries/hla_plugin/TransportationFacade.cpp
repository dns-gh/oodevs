// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TransportationFacade.h"
#include "TransportationController.h"
#include "NetnRequestConvoy.h"
#include "NetnRequestConvoySender.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TransportationFacade constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
TransportationFacade::TransportationFacade( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                            tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                            const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                            Federate_ABC& federate, const ContextFactory_ABC& contextFactory )
    : pTransportationController_( new TransportationController( xis, missionResolver, controller, callsignResolver, subordinates ) )
    , pNetnRequestConvoy_       ( new NetnRequestConvoy( federate ) )
    , pNetnRequestConvoySender_ ( new NetnRequestConvoySender( *pTransportationController_, *pNetnRequestConvoy_, contextFactory ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
TransportationFacade::~TransportationFacade()
{
    // NOTHING
}
