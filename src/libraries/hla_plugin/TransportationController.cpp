// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TransportationController.h"
#include "MissionResolver_ABC.h"
#include "TransportationListener_ABC.h"
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    unsigned int ResolveMission( xml::xisubstream xis, const MissionResolver_ABC& resolver )
    {
        std::string name;
        xis >> xml::start( "missions" )
                >> xml::content( "transport", name );
        return resolver.Resolve( name );
    }
}

// -----------------------------------------------------------------------------
// Name: TransportationController::oller
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationController::TransportationController( xml::xisubstream xis, const MissionResolver_ABC& resolver,
                                                    tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                                    TransportationListener_ABC& listener )
    : transportIdentifier_( ResolveMission( xis, resolver ) )
    , listener_           ( listener )
{
    CONNECT( controller, *this, automat_order );
}

// -----------------------------------------------------------------------------
// Name: TransportationController destructor
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
TransportationController::~TransportationController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationController::Notify
// Created: SLI 2011-10-06
// -----------------------------------------------------------------------------
void TransportationController::Notify(  const sword::AutomatOrder& message, int /*context*/ )
{
    if( message.type().id() == transportIdentifier_ )
        listener_.ConvoyRequested();
}
