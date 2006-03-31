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

// -----------------------------------------------------------------------------
// Name: ActionController constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::ActionController()
    : selecting_( false )
    , popupMenu_( new QPopupMenu() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionController destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ActionController::~ActionController()
{
    delete popupMenu_;
}

// -----------------------------------------------------------------------------
// Name: ActionController::Register
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ActionController::Register( Observer_ABC& observer )
{
//    std::cout << typeid( observer ).name() << " at " << &observer << std::endl;
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
    if( popupMenu_->count() )
        popupMenu_->popup( where );
}
