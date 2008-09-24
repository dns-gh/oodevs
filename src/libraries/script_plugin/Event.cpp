// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Event.h"
#include "Condition_ABC.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: Event constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Event::Event( boost::shared_ptr< Condition_ABC > condition, const std::vector< std::string >& states, const directia::WrappedScriptFunction& function )
    : condition_( condition )
{
    std::copy( states.begin(), states.end(), std::inserter( states_, states_.begin() ) );
    condition_->SetTarget( function );
}

// -----------------------------------------------------------------------------
// Name: Event destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Event::~Event()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event::ChangeState
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
void Event::ChangeState( const std::string& state )
{
    condition_->Activate( states_.empty() || states_.find( state ) != states_.end() );
}

// -----------------------------------------------------------------------------
// Name: Event::Deactivate
// Created: AGE 2008-07-24
// -----------------------------------------------------------------------------
void Event::Deactivate()
{
    condition_->Activate( false );
}

