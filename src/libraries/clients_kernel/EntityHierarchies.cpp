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
#include "Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
EntityHierarchies::EntityHierarchies( Controller& controller )
    : controller_( controller )
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
// Name: EntityHierarchies::CreateSubordinateIterator
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
    const Entity_ABC* superior = GetSuperior();
    if( superior == & entity )
        return true;
    if( superior == 0 )
        return false;
    const Hierarchies* hierarchies = superior->Retrieve< Hierarchies >();
    return hierarchies && hierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::AddSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void EntityHierarchies::AddSubordinate( Entity_ABC& entity )
{
    Register( entity.GetId(), entity );
    controller_.Update( *(Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RemoveSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void EntityHierarchies::RemoveSubordinate( const Entity_ABC& entity )
{
    Remove( entity.GetId() );
    controller_.Update( *(Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::UnregisterSubordinate
// Created: SBO 2006-10-03
// -----------------------------------------------------------------------------
void EntityHierarchies::UnregisterSubordinate( const Entity_ABC& entity )
{
    Remove( entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetTop
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
const Entity_ABC& EntityHierarchies::GetTop() const
{
    const Entity_ABC* superior = GetSuperior();
    if( superior )
    {
        const Hierarchies* superiorHierarchies = superior->Retrieve< Hierarchies >();
        if( superiorHierarchies )
            return superiorHierarchies->GetTop();
        return *superior;
    }
    return GetEntity();
}
