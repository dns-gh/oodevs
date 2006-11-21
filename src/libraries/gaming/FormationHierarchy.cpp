// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FormationHierarchy.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationHierarchy constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
FormationHierarchy::FormationHierarchy( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
    : EntityHierarchies< TacticalHierarchies >( controller, entity, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchy destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
FormationHierarchy::~FormationHierarchy()
{
    // NOTHING
}