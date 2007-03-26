// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionController constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::ActionController()
    : selecting_( false )
    , menu_()
{
    menu_.AddCategory( "Parameter" );
    menu_.AddCategory( "Creation" );
    menu_.AddCategory( "Interface" );
    menu_.AddCategory( "Order" );
    menu_.AddCategory( "Command" );
    menu_.AddCategory( "Helpers" );
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
// Name: ActionController::ShowMenu
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void ActionController::ShowMenu( const QPoint& where )
{
    menu_.Popup( where );
}
