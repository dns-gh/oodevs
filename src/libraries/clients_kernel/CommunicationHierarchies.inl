// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
// *****************************************************************************

#include "Entity_ABC.h"
#include "Controller.h"
#include "PropertiesDictionary.h"
#include "Tools.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
CommunicationHierarchies< Interface >::CommunicationHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
    : Creatable< Interface >( controller, this )
    , controller_( controller )
    , entity_    ( entity )
    , superior_  ( superior )
{
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->RegisterSubordinate( entity_ );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies destructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
CommunicationHierarchies< Interface >::~CommunicationHierarchies()
{
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->UnregisterSubordinate( entity_ );
    tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        Interface* child = const_cast< Entity_ABC& >( it.NextElement() ).Retrieve< Interface >();
        if( child )
            child->UnregisterParent();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
const Entity_ABC* CommunicationHierarchies< Interface >::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetEntity
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
const Entity_ABC& CommunicationHierarchies< Interface >::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CreateSubordinateIterator
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
tools::Iterator< const Entity_ABC& > CommunicationHierarchies< Interface >::CreateSubordinateIterator() const
{
    return CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RegisterSubordinate
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::RegisterSubordinate( Entity_ABC& entity )
{
    Register( entity.GetId(), entity );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::AddSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::AddSubordinate( Entity_ABC& entity )
{
    RegisterSubordinate( entity );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RemoveSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::RemoveSubordinate( const Entity_ABC& entity )
{
    UnregisterSubordinate( entity );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::UnregisterSubordinate
// Created: SBO 2006-10-03
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::UnregisterSubordinate( const Entity_ABC& entity )
{
    Remove( entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::UnregisterParent
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::UnregisterParent()
{
    SetSuperiorInternal( 0 );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::ChangeSuperior
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::ChangeSuperior( Entity_ABC* superior )
{
    if( GetSuperior() == superior )
        return;
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->RemoveSubordinate( entity_ );
    SetSuperiorInternal( superior );
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->AddSubordinate( entity_ );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::SetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::SetSuperior( Entity_ABC* superior )
{
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->UnregisterSubordinate( entity_ );
    SetSuperiorInternal( superior );
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->RegisterSubordinate( entity_ );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::SetSuperiorInternal
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::SetSuperiorInternal( Entity_ABC* superior )
{
    superior_ = superior;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RetrieveHierarchies
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
template< typename Interface >
const Hierarchies* CommunicationHierarchies< Interface >::RetrieveHierarchies( const Entity_ABC& entity ) const
{
    return entity.Retrieve< Interface >();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::SuperiorHierarchy
// Created: AGE 2006-10-09
// -----------------------------------------------------------------------------
template< typename Interface >
Interface* CommunicationHierarchies< Interface >::SuperiorHierarchy()
{
    Entity_ABC* superior = const_cast< Entity_ABC* >( GetSuperior() );
    return superior ? superior->Retrieve< Interface >() : 0;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
template< typename Interface >
void CommunicationHierarchies< Interface >::CreateDictionary( PropertiesDictionary& dico ) const
{
    dico.Register( *(const Interface*)this, tools::translate( "EntityHierarchies", "Hierarchies/Superior" ), superior_ );
}

}

