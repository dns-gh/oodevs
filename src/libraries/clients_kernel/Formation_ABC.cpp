// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Formation_ABC.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation_ABC constructor
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
Formation_ABC::Formation_ABC()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Formation_ABC destructor
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
Formation_ABC::~Formation_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Select
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Formation_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::ContextMenu
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Formation_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Activate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Formation_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}
