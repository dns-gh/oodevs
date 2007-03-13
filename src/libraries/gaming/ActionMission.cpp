// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionMission.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::ActionMission( kernel::Entity_ABC& target, const kernel::Mission& mission, kernel::Controller& controller )
    : Action_ABC( controller )
    , target_( target )
    , mission_( mission )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionMission destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::~ActionMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionMission::GetName
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
QString ActionMission::GetName() const
{
    return mission_.GetName();
}
