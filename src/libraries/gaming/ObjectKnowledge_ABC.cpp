// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledge_ABC.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;

const QString ObjectKnowledge_ABC::typeName_ = "objectKnowledge";

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectKnowledge_ABC::ObjectKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectKnowledge_ABC::~ObjectKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::GetTypeName
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
QString ObjectKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::Select
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::ContextMenu
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::Activate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}
