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
#include "PropertiesDictionary.h"

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
EntityHierarchies< Interface >::EntityHierarchies( Controller& controller, Entity_ABC& entity )
    : controller_( controller )
    , entity_    ( entity )
    , superior_  ( 0 )
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
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->UnregisterSubordinate( entity_ );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
const Entity_ABC* EntityHierarchies< Interface >::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetEntity
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
const Entity_ABC& EntityHierarchies< Interface >::GetEntity() const
{
    return entity_;
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
// Name: EntityHierarchies::SetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::SetSuperior( Entity_ABC* superior )
{
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
            superiorHierarchy->RemoveSubordinate( entity_ );
    superior_ = superior;
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
            superiorHierarchy->AddSubordinate( entity_ );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RetrieveHierarchies
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
template< typename Interface >
const Hierarchies* EntityHierarchies< Interface >::RetrieveHierarchies( const Entity_ABC& entity ) const
{
    return entity.Retrieve< Interface >();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::SuperiorHierarchy
// Created: AGE 2006-10-09
// -----------------------------------------------------------------------------
template< typename Interface >
Interface* EntityHierarchies< Interface >::SuperiorHierarchy()
{
    Entity_ABC* superior = const_cast< Entity_ABC* >( GetSuperior() );
    return superior ? superior->Retrieve< Interface >() : 0;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *(const Interface*)this, tools::translate( "EntityHierarchies", "Hiérarchie/Supérieur" ), superior_ );
}

}

