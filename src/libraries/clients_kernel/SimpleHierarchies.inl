// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace kernel
{
// -----------------------------------------------------------------------------
// Name: SimpleHierarchies constructor
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
SimpleHierarchies< I >::SimpleHierarchies( const Entity_ABC& holder, const Entity_ABC* superior )
     : holder_( holder )
     , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies destructor
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
SimpleHierarchies< I >::~SimpleHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::GetSuperior
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
const Entity_ABC* SimpleHierarchies< I >::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::GetEntity
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
const Entity_ABC& SimpleHierarchies< I >::GetEntity() const
{
    return holder_;
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator< const Entity_ABC& > SimpleHierarchies::CreateSubordinateIterator
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
tools::Iterator< const Entity_ABC& > SimpleHierarchies< I >::CreateSubordinateIterator() const
{
    return new tools::NullIterator< const Entity_ABC& >();
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::RegisterSubordinate
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
void SimpleHierarchies< I >::RegisterSubordinate( Entity_ABC& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::AddSubordinate
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
void SimpleHierarchies< I >::AddSubordinate( Entity_ABC& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::RemoveSubordinate
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
void SimpleHierarchies< I >::RemoveSubordinate( const Entity_ABC& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::UnregisterSubordinate
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
void SimpleHierarchies< I >::UnregisterSubordinate( const Entity_ABC& )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::UnregisterParent
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
void SimpleHierarchies< I >::UnregisterParent()
{
    superior_ = 0;
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::RetrieveHierarchies
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
const Hierarchies* SimpleHierarchies< I >::RetrieveHierarchies( const Entity_ABC& entity ) const
{
    return entity.Retrieve< I >();
}

// -----------------------------------------------------------------------------
// Name: SimpleHierarchies::SetSuperior
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
template< typename I >
void SimpleHierarchies< I >::SetSuperior( const Entity_ABC* superior )
{
    superior_ = superior;
}

}