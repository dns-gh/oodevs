// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "EntityHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: EntityHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
EntityHierarchies::EntityHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::EntityHierarchies( controller )
    , controller_( controller )
    , holder_( holder )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies destructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
EntityHierarchies::~EntityHierarchies()
{
    DeleteAll();
    if( superior_ )
        if( kernel::Hierarchies* hierarchies = superior_->Retrieve< kernel::Hierarchies >() )
            hierarchies->UnregisterSubordinate( holder_ );
    controller_.Delete( *(kernel::Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::DoUpdate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void EntityHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    RegisterToSuperior();
    controller_.Create( *(kernel::Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetSuperior
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* EntityHierarchies::GetSuperior() const
{
    return superior_;
}
    
// -----------------------------------------------------------------------------
// Name: EntityHierarchies::GetEntity
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& EntityHierarchies::GetEntity() const
{
    return holder_;
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::ChangeSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void EntityHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    RemoveFromSuperior();
    superior_ = &superior;
    RegisterToSuperior();
    controller_.Update( *(kernel::Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RegisterToSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void EntityHierarchies::RegisterToSuperior()
{
    if( superior_ )
        if( kernel::Hierarchies* hierarchies = superior_->Retrieve< kernel::Hierarchies >() )
            hierarchies->AddSubordinate( holder_ );
}
    
// -----------------------------------------------------------------------------
// Name: EntityHierarchies::RemoveFromSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void EntityHierarchies::RemoveFromSuperior()
{
    if( superior_ )
        if( kernel::Hierarchies* hierarchies = superior_->Retrieve< kernel::Hierarchies >() )
        {
            hierarchies->RemoveSubordinate( holder_ );
            superior_ = 0;
        }
}
