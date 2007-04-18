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
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: ActionMission constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::ActionMission( const kernel::Entity_ABC& entity, const kernel::Mission& mission, kernel::Controller& controller, bool registered /* = true */ )
    : Action_ABC( controller )
    , controller_( controller )
    , entity_( entity )
    , mission_( mission )
    , registered_( registered )
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionMission destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionMission::~ActionMission()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionMission::GetName
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
QString ActionMission::GetName() const
{
    return mission_.GetName();
}

// -----------------------------------------------------------------------------
// Name: ActionMission::GetEntity
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& ActionMission::GetEntity() const
{
    return entity_;
}
