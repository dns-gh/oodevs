// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Ghost_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString Ghost_ABC::typeName_ = "ghost";

// -----------------------------------------------------------------------------
// Name: Ghost_ABC constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
Ghost_ABC::Ghost_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Ghost_ABC destructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
Ghost_ABC::~Ghost_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Ghost_ABC::GetTypeName
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
QString Ghost_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Ghost_ABC::Select
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void Ghost_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Ghost_ABC::ContextMenu
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void Ghost_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Ghost_ABC::Activate
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void Ghost_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Ghost_ABC::OverFly
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
void Ghost_ABC::OverFly( ActionController& controller ) const
{
    controller.OverFly( *this, *(const Entity_ABC*)this );
}
