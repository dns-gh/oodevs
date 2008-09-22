// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Drawing_ABC.h"
#include "clients_kernel/ActionController.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Drawing_ABC constructor
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
Drawing_ABC::Drawing_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC destructor
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
Drawing_ABC::~Drawing_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::Select
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void Drawing_ABC::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this, *(const kernel::Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::ContextMenu
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void Drawing_ABC::ContextMenu( kernel::ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const kernel::Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::Activate
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void Drawing_ABC::Activate( kernel::ActionController& controller ) const
{
    controller.Activate( *this, *(const kernel::Entity_ABC*)this );
}
