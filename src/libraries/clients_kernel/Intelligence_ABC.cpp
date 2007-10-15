// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Intelligence_ABC.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Intelligence_ABC constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence_ABC::Intelligence_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence_ABC destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence_ABC::~Intelligence_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence_ABC::Select
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void Intelligence_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Intelligence_ABC::ContextMenu
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void Intelligence_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Intelligence_ABC::Activate
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void Intelligence_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
