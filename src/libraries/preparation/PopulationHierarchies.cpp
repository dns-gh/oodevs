// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PopulationHierarchies.h"
#include "Population.h"
#include "Populations.h"

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
PopulationHierarchies::PopulationHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
PopulationHierarchies::~PopulationHierarchies()
{
    if( GetSuperior() )
        if( const Populations* populations = GetSuperior()->Retrieve< Populations >() )
            const_cast< Populations* >( populations )->Remove( GetEntity().GetId() );
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies::ChangeSuperior
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
void PopulationHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    if( kernel::Entity_ABC* oldsuperior = const_cast< kernel::Entity_ABC* >( GetSuperior() ) )
        if( Populations* populations = oldsuperior->Retrieve< Populations >() )
            populations->Remove( GetEntity().GetId() );
    SetSuperior( &superior );
    if( Populations* populations = superior.Retrieve< Populations >() )
    {
        const Population& pop = static_cast< const Population& >( GetEntity() );
        populations->Add( const_cast< Population& >( pop ) );
    }
}
