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
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationHierarchy constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
FormationHierarchy::FormationHierarchy( Controller& controller, Formation_ABC& entity, Entity_ABC* superior )
    : MergingTacticalHierarchies( controller, entity, superior )
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
