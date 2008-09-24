// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "AutomatConditions.h"
#include "SimpleEntityCondition.h"
#include "AgentEvents.h"
#include "directia/Brain.h"
#include "dispatcher/Automat.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: AutomatConditions constructor
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
AutomatConditions::AutomatConditions( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatConditions destructor
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
AutomatConditions::~AutomatConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatConditions::RegisterIn
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
void AutomatConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.automats", this );
    brain.RegisterFunction( "MissionStarted", &AutomatConditions::MissionStarted );
}

// -----------------------------------------------------------------------------
// Name: AutomatConditions::MissionStarted
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > AutomatConditions::MissionStarted()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::AutomatMissionStarted >( controller_ ) );
}
