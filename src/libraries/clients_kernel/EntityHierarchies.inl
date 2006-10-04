// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Entity_ABC.h"
#include "Controller.h"

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
EntityHierarchies< Interface >::EntityHierarchies( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies destructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
EntityHierarchies< Interface >::~EntityHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CreateSubordinateIterator
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
Iterator< const Entity_ABC& > EntityHierarchies< Interface >::CreateSubordinateIterator() const
{
    return CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::IsSubordinateOf
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
bool EntityHierarchies< Interface >::IsSubordinateOf( const Entity_ABC& entity ) const
{
    const Entity_ABC* superior = GetSuperior();
    if( superior == & entity )
        return true;
    if( superior == 0 )
        return false;
    const Interface* hierarchies = superior->Retrieve< Interface >();
    return hierarchies && hierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::AddSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::AddSubordinate( Entity_ABC& entity )
{
    Register( entity.GetId(), entity );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RemoveSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::RemoveSubordinate( const Entity_ABC& entity )
{
    UnregisterSubordinate( entity );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::UnregisterSubordinate
// Created: SBO 2006-10-03
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::UnregisterSubordinate( const Entity_ABC& entity )
{
    Remove( entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetTop
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename Interface >
const Entity_ABC& EntityHierarchies< Interface >::GetTop() const
{
    const Entity_ABC* superior = GetSuperior();
    if( superior )
    {
        const Interface* superiorHierarchies = superior->Retrieve< Interface >();
        if( superiorHierarchies )
            return superiorHierarchies->GetTop();
        return *superior;
    }
    return GetEntity();
}

}
