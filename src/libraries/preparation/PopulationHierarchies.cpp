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
#include "Populations.h"

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
PopulationHierarchies::PopulationHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, holder )
    , superior_( superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PopulationHierarchies destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
PopulationHierarchies::~PopulationHierarchies()
{
    if( superior_ )
        if( Populations* popus = superior_->Retrieve< Populations >() )
            popus->Remove( GetEntity().GetId() );
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies::GetSuperior
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* PopulationHierarchies::GetSuperior() const
{
    return superior_;
}