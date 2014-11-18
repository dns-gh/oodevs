// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingHierarchies.h"
#include "clients_kernel/Drawing_ABC.h"

// -----------------------------------------------------------------------------
// Name: DrawingHierarchies constructor
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
DrawingHierarchies::DrawingHierarchies( const kernel::Drawing_ABC& drawing )
     : drawing_( drawing )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingHierarchies destructor
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
DrawingHierarchies::~DrawingHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::GetEntity
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& DrawingHierarchies::GetEntity() const
{
    return drawing_;
}
    
// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::GetSuperior
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* DrawingHierarchies::GetSuperior() const
{
    return drawing_.GetDiffusionEntity();
}
    
// -----------------------------------------------------------------------------
// Name: tools::Iterator< const Entity_ABC& > DrawingHierarchies::CreateSubordinateIterator
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::Entity_ABC& > DrawingHierarchies::CreateSubordinateIterator() const
{
    return new tools::NullIterator< const kernel::Entity_ABC& >();
}
    
// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::RegisterSubordinate
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
void DrawingHierarchies::RegisterSubordinate( kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::AddSubordinate
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
void DrawingHierarchies::AddSubordinate( kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::RemoveSubordinate
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
void DrawingHierarchies::RemoveSubordinate( const kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::UnregisterSubordinate
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
void DrawingHierarchies::UnregisterSubordinate( const kernel::Entity_ABC& )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}
    
// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::UnregisterParent
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
void DrawingHierarchies::UnregisterParent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingHierarchies::RetrieveHierarchies
// Created: LDC 2014-11-12
// -----------------------------------------------------------------------------
const kernel::Hierarchies* DrawingHierarchies::RetrieveHierarchies( const kernel::Entity_ABC& entity ) const
{
    return entity.Retrieve< kernel::TacticalHierarchies >();
}
