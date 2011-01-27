// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Inhabitant_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString Inhabitant_ABC::typeName_ = "inhabitant";

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC constructor
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
Inhabitant_ABC::Inhabitant_ABC()
    : healthy_( 0 )
    , dead_   ( 0 )
    , wounded_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC::~Inhabitant_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::GetTypeName
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
QString Inhabitant_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::Select
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::ContextMenu
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::Activate
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
