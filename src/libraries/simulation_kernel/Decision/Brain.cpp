// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Brain.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: Brain constructor
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
Brain::Brain( const std::string& config )
    : brain_( new directia::brain::Brain( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Brain constructor
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
Brain::Brain( Brain& parent )
    : brain_( new directia::brain::Brain( *parent.brain_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Brain::SelectActions
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
void Brain::SelectActions()
{
    brain_->SelectActions();
}

// -----------------------------------------------------------------------------
// Name: Brain::TriggerSelectedActions
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
void Brain::TriggerSelectedActions( double elapsedTime /*= 1*/ ) const
{
    brain_->TriggerSelectedActions( elapsedTime );
}

// -----------------------------------------------------------------------------
// Name: Brain::GetScriptRef
// Created: MCO 2012-10-31
// -----------------------------------------------------------------------------
directia::tools::binders::ScriptRef Brain::GetScriptRef( const std::string& s )
{
    return (*brain_)[ s ];
}

// -----------------------------------------------------------------------------
// Name: Brain::GetScriptRef
// Created: MCO 2012-11-02
// -----------------------------------------------------------------------------
directia::tools::binders::ScriptRef Brain::GetScriptRef()
{
    return (*brain_);
}
