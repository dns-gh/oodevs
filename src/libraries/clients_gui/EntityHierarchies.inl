// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

namespace gui
{

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
EntityHierarchies< Interface >::EntityHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::Entity_ABC* superior )
    : kernel::Creatable< Interface >( controller, this )
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
EntityHierarchies< Interface >::~EntityHierarchies()
{
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->UnregisterSubordinate( entity_ );
    tools::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        Interface* child = const_cast< kernel::Entity_ABC& >( it.NextElement() ).Retrieve< Interface >();
        if( child && child->GetSuperior() == &entity_ )
            child->UnregisterParent();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
const kernel::Entity_ABC* EntityHierarchies< Interface >::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetEntity
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
const kernel::Entity_ABC& EntityHierarchies< Interface >::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CreateSubordinateIterator
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
template< typename Interface >
tools::Iterator< const kernel::Entity_ABC& > EntityHierarchies< Interface >::CreateSubordinateIterator() const
{
    return CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::CountSubordinates
// Created: RPD 2011-11-07
// -----------------------------------------------------------------------------
template< typename Interface >
long EntityHierarchies< Interface >::CountSubordinates() const
{
    return Count();
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RegisterSubordinate
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::RegisterSubordinate( kernel::Entity_ABC& entity )
{
    Register( entity.GetId(), entity );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::AddSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::AddSubordinate( kernel::Entity_ABC& entity )
{
    RegisterSubordinate( entity );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RemoveSubordinate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::RemoveSubordinate( const kernel::Entity_ABC& entity )
{
    UnregisterSubordinate( entity );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::UnregisterSubordinate
// Created: SBO 2006-10-03
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::UnregisterSubordinate( const kernel::Entity_ABC& entity )
{
    Remove( entity.GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::UnregisterParent
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::UnregisterParent()
{
    SetSuperiorInternal( 0 );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::ChangeSuperior
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::ChangeSuperior( kernel::Entity_ABC* superior )
{
    if( GetSuperior() == superior )
        return;
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->RemoveSubordinate( entity_ );
    SetSuperiorInternal( superior );
    if( Interface* superiorHierarchy = SuperiorHierarchy() )
        superiorHierarchy->AddSubordinate( entity_ );
    NotifySuperiorChanged( superior );
    controller_.Update( *(Interface*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::SetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
template< typename Interface >
void EntityHierarchies< Interface >::SetSuperior( kernel::Entity_ABC* superior )
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
void EntityHierarchies< Interface >::SetSuperiorInternal( kernel::Entity_ABC* superior )
{
    superior_ = superior;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RetrieveHierarchies
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
template< typename Interface >
const kernel::Hierarchies* EntityHierarchies< Interface >::RetrieveHierarchies( const kernel::Entity_ABC& entity ) const
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
    kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( GetSuperior() );
    return superior ? superior->Retrieve< Interface >() : 0;
}

}
