// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ProfileHierarchies.h"

// -----------------------------------------------------------------------------
// Name: ProfileHierarchies constructor
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
ProfileHierarchies::ProfileHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::EntityHierarchies< ProfileHierarchies_ABC >( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileHierarchies destructor
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
ProfileHierarchies::~ProfileHierarchies()
{
    // NOTHING
}
