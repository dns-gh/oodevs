// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ScriptCommands.h"
#include "MiscEvents.h"
#include "directia/brain/Brain.h"
#include "clients_kernel/Controller.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ScriptCommands constructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
ScriptCommands::ScriptCommands( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScriptCommands destructor
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
ScriptCommands::~ScriptCommands()
{
    // NOTHING
}

namespace directia
{
    void UsedByDIA( ScriptCommands* ) {}
    void ReleasedByDIA( ScriptCommands* ) {}
}

// -----------------------------------------------------------------------------
// Name: ScriptCommands::RegisterIn
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void ScriptCommands::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "script" ] = this;
    brain.Register( "ChangePhase", &ScriptCommands::ChangePhase );
}

// -----------------------------------------------------------------------------
// Name: ScriptCommands::ChangePhase
// Created: SBO 2008-07-02
// -----------------------------------------------------------------------------
void ScriptCommands::ChangePhase( const std::string& phase )
{
    controller_.Update( events::PhaseChanged( phase ) );
}
