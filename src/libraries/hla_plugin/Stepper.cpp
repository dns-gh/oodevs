// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Stepper.h"
#include "protocol/SimulationSenders.h"
#include "protocol/Simulation.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Stepper constructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
Stepper::Stepper( xml::xistream& xis, MessageDispatcher_ABC< sword::SimToClient_Content >& controller, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_( publisher )
{
    if( xis.attribute< bool >( "time-constrained", true ) )
        CONNECT( controller, sword::ControlEndTick, control_end_tick );
}

// -----------------------------------------------------------------------------
// Name: Stepper destructor
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
Stepper::~Stepper()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Stepper::Notify
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
void Stepper::Notify( const sword::ControlEndTick& /*message*/ )
{
    simulation::ControlResume message;
    message().set_tick( 1 );
    message.Send( publisher_ );
}
