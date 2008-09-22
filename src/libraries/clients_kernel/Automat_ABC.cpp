// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Automat_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString Automat_ABC::typeName_ = "automat";

// -----------------------------------------------------------------------------
// Name: Automat_ABC constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat_ABC::Automat_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat_ABC destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat_ABC::~Automat_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
QString Automat_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Automat_ABC::Select
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Automat_ABC::ContextMenu
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Automat_ABC::Activate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
