// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ActionController.h"
#include "ActionObserver_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionController constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::ActionController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionController destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::~ActionController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionController::Register
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ActionController::Register( Observer_ABC& observer )
{
    InterfaceContainer< Observer_ABC >::Register( observer );
}

// -----------------------------------------------------------------------------
// Name: ActionController::Remove
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ActionController::Remove( Observer_ABC& observer )
{
    InterfaceContainer< Observer_ABC >::Remove( observer );
}

// -----------------------------------------------------------------------------
// Name: ActionController::Select
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ActionController::Select( const Agent& agent )
{
    Apply( ActionObserver_ABC::NotifySelected, agent );
}
