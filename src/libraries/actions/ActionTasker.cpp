// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionTasker.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionTasker constructor
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
ActionTasker::ActionTasker( const kernel::Entity_ABC& tasker, bool simulation )
    : tasker_( tasker )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker destructor
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
ActionTasker::~ActionTasker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::GetTasker
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& ActionTasker::GetTasker() const
{
    return tasker_;
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::IsSimulation
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
bool ActionTasker::IsSimulation() const
{
    return simulation_;
}
