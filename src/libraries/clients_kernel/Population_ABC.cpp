// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Population_ABC.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Population_ABC constructor
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
Population_ABC::Population_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Population_ABC destructor
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
Population_ABC::~Population_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::Select
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Population_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::ContextMenu
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Population_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::Activate
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Population_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
