// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "InhabitantHierarchies.h"

// -----------------------------------------------------------------------------
// Name: InhabitantHierarchies constructor
// Created: AGE 2010-11-29
// -----------------------------------------------------------------------------
InhabitantHierarchies::InhabitantHierarchies( const kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior )
     : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, &superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantHierarchies destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
InhabitantHierarchies::~InhabitantHierarchies()
{
    // NOTHING
}
