// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EntityHierarchies.h"
#include "Entity_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
EntityHierarchies::EntityHierarchies()
    : superior_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies destructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
EntityHierarchies::~EntityHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetSuperior
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
const Entity_ABC* EntityHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: Iterator< const Entity_ABC& > EntityHierarchies::CreateSubordinateIterator
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
Iterator< const Entity_ABC& > EntityHierarchies::CreateSubordinateIterator() const
{
    return CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::IsSubordinateOf
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
bool EntityHierarchies::IsSubordinateOf( const Entity_ABC& entity ) const
{
    if( superior_ == & entity )
        return true;
    if( superior_ == 0 )
        return false;
    const Hierarchies* hierarchies = superior_->Retrieve< Hierarchies >();
    return hierarchies && hierarchies->IsSubordinateOf( entity );
}
