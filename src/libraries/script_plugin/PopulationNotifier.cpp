// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "PopulationNotifier.h"
#include "AgentEvents.h"
#include "dispatcher/Population.h"
#include "clients_kernel/Controller.h"

using namespace plugins::script;
using namespace events;

// -----------------------------------------------------------------------------
// Name: PopulationNotifier constructor
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
PopulationNotifier::PopulationNotifier( kernel::Controller& controller, const dispatcher::Population& population )
    : controller_( controller )
    , population_( population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationNotifier destructor
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
PopulationNotifier::~PopulationNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationNotifier::DoUpdate
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
void PopulationNotifier::DoUpdate( const ASN1T_MsgPopulationFlowUpdate& message )
{
    if( (unsigned int)message.oid_population == population_.GetId() && message.m.fluxPresent )
        controller_.Update( events::PopulationMoved( population_ ) );
}
