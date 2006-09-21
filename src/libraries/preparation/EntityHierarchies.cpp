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
    if( superior_ )
        if( Hierarchies* hierarchies = superior_->Retrieve< Hierarchies >() )
            hierarchies->RemoveSubordinate( holder_ );
    controller_.Delete( *(kernel::Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityHierarchies::DoUpdate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void EntityHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    if( superior_ )
        if( Hierarchies* hierarchies = superior_->Retrieve< Hierarchies >() )
            hierarchies->AddSubordinate( holder_ );
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
