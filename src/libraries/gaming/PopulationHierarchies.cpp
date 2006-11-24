// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationHierarchies.h"

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies constructor
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
PopulationHierarchies::PopulationHierarchies( const kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior )
     : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, &superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies destructor
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
PopulationHierarchies::~PopulationHierarchies()
{
    // NOTHING
}
