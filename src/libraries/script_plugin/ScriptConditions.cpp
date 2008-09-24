// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ScriptConditions.h"
#include "EventCondition.h"
#include "MiscEvents.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "directia/Brain.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ScriptConditions constructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
ScriptConditions::ScriptConditions( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScriptConditions destructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
ScriptConditions::~ScriptConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScriptConditions::RegisterIn
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void ScriptConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.script", this );
    brain.RegisterFunction( "PhaseChanged", &ScriptConditions::PhaseChanged );
}

// -----------------------------------------------------------------------------
// Name: ScriptConditions::PhaseChanged
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ScriptConditions::PhaseChanged()
{
    struct PhaseChanged : public SimpleEventCondition< events::PhaseChanged >
    {
        PhaseChanged( kernel::Controller& controller )
            : SimpleEventCondition( controller ) {}

        virtual void NotifyUpdated( const events::PhaseChanged& ev )
        {
            Trigger( ev.phase );
        };
    };

    return boost::shared_ptr< Condition_ABC >( new PhaseChanged( controller_ ) );
}
