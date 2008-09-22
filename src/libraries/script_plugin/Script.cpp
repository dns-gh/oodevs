// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Script.h"
#include "FiniteStateMachine.h"
#include "dispatcher/Registrable_ABC.h"
#include "directia/Brain.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace script;

// -----------------------------------------------------------------------------
// Name: Script constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Script::Script( const std::string& file, dispatcher::Registrable_ABC& registrables )
    : file_ ( file )
    , brain_( new directia::Brain( file, "brain" ) )
    , fsm_  ( new FiniteStateMachine( *brain_ ) )
{
    registrables.RegisterIn( *brain_ );
    brain_->GetScriptFunction( "Start" )();
}

// -----------------------------------------------------------------------------
// Name: Script destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Script::~Script()
{
    // NOTHING
}