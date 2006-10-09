// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies constructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
TacticalHierarchies::TacticalHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller )
    , controller_( controller )
    , holder_( holder )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies destructor
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
TacticalHierarchies::~TacticalHierarchies()
{
    DeleteAll();
    if( superior_ )
        if( kernel::TacticalHierarchies* hierarchies = superior_->Retrieve< kernel::TacticalHierarchies >() )
            hierarchies->UnregisterSubordinate( holder_ );
    controller_.Delete( *(kernel::TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies::DoUpdate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void TacticalHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    RegisterToSuperior();
    controller_.Create( *(kernel::TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies::ChangeSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    RemoveFromSuperior();
    superior_ = &superior;
    RegisterToSuperior();
    controller_.Update( *(kernel::TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies::RegisterToSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalHierarchies::RegisterToSuperior()
{
    if( superior_ )
        if( kernel::TacticalHierarchies* hierarchies = superior_->Retrieve< kernel::TacticalHierarchies >() )
            hierarchies->AddSubordinate( holder_ );
}
    
// -----------------------------------------------------------------------------
// Name: TacticalHierarchies::RemoveFromSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalHierarchies::RemoveFromSuperior()
{
    if( superior_ )
        if( kernel::TacticalHierarchies* hierarchies = superior_->Retrieve< kernel::TacticalHierarchies >() )
        {
            hierarchies->RemoveSubordinate( holder_ );
            superior_ = 0;
        }
}
