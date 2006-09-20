// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Object_ABC.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Object_ABC constructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
Object_ABC::Object_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ABC destructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
Object_ABC::~Object_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Select
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Object_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::ContextMenu
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Object_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Object_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
