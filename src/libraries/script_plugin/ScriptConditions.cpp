// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ScriptConditions.h"
#include "EventCondition.h"
#include "MiscEvents.h"
#include "tools/ElementObserver_ABC.h"
#include "directia/brain/Brain.h"

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

namespace directia
{
    void UsedByDIA( ScriptConditions* ) {}
    void ReleasedByDIA( ScriptConditions* ) {}
}

// -----------------------------------------------------------------------------
// Name: ScriptConditions::RegisterIn
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void ScriptConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.script" ] = this;
    brain.Register( "PhaseChanged", &ScriptConditions::PhaseChanged );
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
            : SimpleEventCondition( controller )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const events::PhaseChanged& ev )
        {
            Trigger( ev.phase );
        };
    };
    return boost::shared_ptr< Condition_ABC >( new PhaseChanged( controller_ ) );
}
