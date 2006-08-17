// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Controller constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Controller::Controller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Controller destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Controller::~Controller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Controller::Register
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Controller::Register( Observer_ABC& observer )
{
    InterfaceContainer< Observer_ABC >::Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Controller::Remove
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Controller::Remove( Observer_ABC& observer )
{
    InterfaceContainer< Observer_ABC >::Remove( observer );
}
