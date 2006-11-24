// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectHierarchies.h"

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
ObjectHierarchies::ObjectHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectHierarchies destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
ObjectHierarchies::~ObjectHierarchies()
{
    // NOTHING
}

