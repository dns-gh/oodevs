// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PopulationConcentration_ABC.h"
#include "ActionController.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentration_ABC constructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
PopulationConcentration_ABC::PopulationConcentration_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration_ABC destructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
PopulationConcentration_ABC::~PopulationConcentration_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration_ABC::Select
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationConcentration_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration_ABC::ContextMenu
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationConcentration_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration_ABC::Activate
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationConcentration_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}
