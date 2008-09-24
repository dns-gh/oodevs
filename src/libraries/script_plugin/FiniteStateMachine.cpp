// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "FiniteStateMachine.h"
#include "Event.h"
#include "Condition_ABC.h"
#include "directia/Brain.h"
#include <boost/bind.hpp>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: FiniteStateMachine constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
FiniteStateMachine::FiniteStateMachine( directia::Brain& brain )
    : state_   ( "start" )
{
    brain.RegisterObject  ( "fsm", this );
    brain.RegisterFunction( "DeclareEvent", &FiniteStateMachine::DeclareEvent );
    brain.RegisterFunction( "CurrentState", &FiniteStateMachine::CurrentState );
    brain.RegisterFunction( "ChangeState",  &FiniteStateMachine::ChangeState  );
    brain.RegisterFunction( "Deactivate",   &FiniteStateMachine::Deactivate   );
}

// -----------------------------------------------------------------------------
// Name: FiniteStateMachine destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
FiniteStateMachine::~FiniteStateMachine()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FiniteStateMachine::DeclareEvent
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void FiniteStateMachine::DeclareEvent( boost::shared_ptr< Condition_ABC > condition, const std::vector< std::string >& states, const directia::WrappedScriptFunction& function )
{
    events_.push_back( new Event( condition, states, function ) );
    events_.back().ChangeState( state_ );
}

// -----------------------------------------------------------------------------
// Name: FiniteStateMachine::CurrentState
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
std::string FiniteStateMachine::CurrentState() const
{
    return state_;
}

// -----------------------------------------------------------------------------
// Name: FiniteStateMachine::ChangeState
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void FiniteStateMachine::ChangeState( const std::string& state )
{
    state_ = state;
    std::for_each( events_.begin(), events_.end(), boost::bind( &Event::ChangeState, _1, state_ ) );
}

// -----------------------------------------------------------------------------
// Name: FiniteStateMachine::Deactivate
// Created: AGE 2008-07-24
// -----------------------------------------------------------------------------
void FiniteStateMachine::Deactivate()
{
    std::for_each( events_.begin(), events_.end(), boost::bind( &Event::Deactivate, _1 ) );
}

