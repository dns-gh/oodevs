// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Entity_ABC.h"
#include "ExtensionVisitor_ABC.h"

namespace kernel
{
// -----------------------------------------------------------------------------
// Name: Hierarchies::IsSubordinateOf
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
inline
bool Hierarchies::IsSubordinateOf( const Entity_ABC& entity ) const
{
    const Entity_ABC* superior = GetSuperior();
    if( superior == & entity )
        return true;
    if( superior == 0 )
        return false;
    const Hierarchies* hierarchies = RetrieveHierarchies( *superior );
    return hierarchies && hierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::IsSubordinateOf
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
inline
bool Hierarchies::IsSubordinateOf( unsigned long id ) const
{
    const Entity_ABC* superior = GetSuperior();
    if( superior )
    {
        if( superior->GetId() == id )
            return true;
        const Hierarchies* hierarchies = RetrieveHierarchies( *superior );
        return hierarchies && hierarchies->IsSubordinateOf( id );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::GetTop
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
inline
const Entity_ABC& Hierarchies::GetTop() const
{
    if( const Entity_ABC* superior = GetSuperior() )
    {
        const Hierarchies* superiorHierarchies = RetrieveHierarchies( *superior );
        if( superiorHierarchies )
            return superiorHierarchies->GetTop();
        return *superior;
    }
    return GetEntity();
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::GetUp
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
inline
const Entity_ABC& Hierarchies::GetUp( unsigned int nLevel /* = 1*/ ) const
{
    if( nLevel == 0 )
        return GetEntity();
    if( const Entity_ABC* superior = GetSuperior() )
    {
        if( nLevel == 1 )
            return *superior;
        if( const Hierarchies* superiorHierarchies = RetrieveHierarchies( *superior ) )
            return superiorHierarchies->GetUp( nLevel - 1 );
    }
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::Accept
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
template< typename T >
inline
void Hierarchies::Accept( ExtensionVisitor_ABC< T >& visitor ) const
{
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC& subordinate = it.NextElement();
        const T* extension = subordinate.Retrieve< T >();
        if( extension )
            visitor.Visit( *extension );
        const Hierarchies* subHierarchies = RetrieveHierarchies( subordinate );
        if( subHierarchies )
            subHierarchies->Accept( visitor );
    }
}

}