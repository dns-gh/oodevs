// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionFactory.h"
#include "ActionMission.h"
#include "ActionFragOrder.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::~ActionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::Mission& mission ) const
{
    return new ActionMission( target, mission, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrder& fragOrder ) const
{
    return new ActionFragOrder( target, fragOrder, controllers_.controller_ );
}
