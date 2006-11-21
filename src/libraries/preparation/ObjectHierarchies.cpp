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
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, holder, 0 )
    , superior_( superior )
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

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies::GetSuperior
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ObjectHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies::DoUpdate
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void ObjectHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    // NOTHING
}
