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

using namespace kernel;

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
void Controller::Register( tools::Observer_ABC& observer )
{
    tools::SortedInterfaceContainer< tools::Observer_ABC >::Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Controller::Unregister
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Controller::Unregister( tools::Observer_ABC& observer )
{
    tools::SortedInterfaceContainer< tools::Observer_ABC >::Unregister( observer );
}
