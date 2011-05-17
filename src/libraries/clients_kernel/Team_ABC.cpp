// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Team_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString Team_ABC::typeName_ = "party";

// -----------------------------------------------------------------------------
// Name: Team_ABC constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Team_ABC::Team_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team_ABC destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Team_ABC::~Team_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
QString Team_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::ContextMenu
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::Activate
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}

