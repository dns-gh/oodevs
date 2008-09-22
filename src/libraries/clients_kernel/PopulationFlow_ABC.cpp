// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PopulationFlow_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString PopulationFlow_ABC::typeName_ = "populationFlow";

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC constructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
PopulationFlow_ABC::PopulationFlow_ABC()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC destructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
PopulationFlow_ABC::~PopulationFlow_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
QString PopulationFlow_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::Select
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::ContextMenu
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::Activate
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
