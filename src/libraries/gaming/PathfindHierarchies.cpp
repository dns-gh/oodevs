// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PathfindHierarchies.h"
#include "clients_kernel/Pathfind_ABC.h"

// -----------------------------------------------------------------------------
// Name: PathfindHierarchies constructor
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
PathfindHierarchies::PathfindHierarchies( const kernel::Pathfind_ABC& pathfind )
     : pathfind_( pathfind )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathfindHierarchies destructor
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
PathfindHierarchies::~PathfindHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::GetEntity
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& PathfindHierarchies::GetEntity() const
{
    return pathfind_;
}
    
// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::GetSuperior
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* PathfindHierarchies::GetSuperior() const
{
    return &pathfind_.GetUnit();
}
    
// -----------------------------------------------------------------------------
// Name: tools::Iterator< const Entity_ABC& > PathfindHierarchies::CreateSubordinateIterator
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::Entity_ABC& > PathfindHierarchies::CreateSubordinateIterator() const
{
    return new tools::NullIterator< const kernel::Entity_ABC& >();
}
    
// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::RegisterSubordinate
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void PathfindHierarchies::RegisterSubordinate( kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::AddSubordinate
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void PathfindHierarchies::AddSubordinate( kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::RemoveSubordinate
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void PathfindHierarchies::RemoveSubordinate( const kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::UnregisterSubordinate
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void PathfindHierarchies::UnregisterSubordinate( const kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::UnregisterParent
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void PathfindHierarchies::UnregisterParent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathfindHierarchies::RetrieveHierarchies
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
const kernel::Hierarchies* PathfindHierarchies::RetrieveHierarchies( const kernel::Entity_ABC& entity ) const
{
    return entity.Retrieve< kernel::TacticalHierarchies >();
}
