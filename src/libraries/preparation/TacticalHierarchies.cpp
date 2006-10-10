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
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, holder )
    , controller_( controller )
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
    DeleteAll(); // $$$$ SBO 2006-10-09: 
    controller_.Delete( *(kernel::TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies::DoUpdate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void TacticalHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    SetSuperior( superior_ );
    controller_.Create( *(kernel::TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TacticalHierarchies::ChangeSuperior
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    SetSuperior( &superior );
    controller_.Update( *(kernel::TacticalHierarchies*)this );
}

