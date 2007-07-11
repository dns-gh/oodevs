// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Controllers.h"
#include "Options.h"
#include "Controller.h"
#include "ActionController.h"

using namespace kernel;

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
// Name: Controllers::Unregister
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void Controllers::Unregister( Observer_ABC& observer )
{
    options_.Unregister( observer );
    controller_.Unregister( observer );
    actions_.Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: Controllers::Update
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void Controllers::Update( Observer_ABC& observer )
{
    Unregister( observer );
    Register  ( observer );
}
