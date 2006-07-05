// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Controllers.h"
#include "Options.h"
#include "Controller.h"
#include "ActionController.h"

// -----------------------------------------------------------------------------
// Name: Controllers constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
Controllers::Controllers()
    : options_( *new Options() )
    , controller_( *new Controller() )
    , actions_( *new ActionController() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Controllers destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
Controllers::~Controllers()
{
    delete &actions_;
    delete &controller_;
    delete &options_;
}

// -----------------------------------------------------------------------------
// Name: Controllers::Register
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void Controllers::Register( Observer_ABC& observer )
{
    options_.Register( observer );
    controller_.Register( observer );
    actions_.Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Controllers::Remove
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void Controllers::Remove( Observer_ABC& observer )
{
    options_.Remove( observer );
    controller_.Remove( observer );
    actions_.Remove( observer );
}
