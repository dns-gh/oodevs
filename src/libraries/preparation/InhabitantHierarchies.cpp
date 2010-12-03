// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InhabitantHierarchies.h"
#include "Inhabitants.h"

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies constructor
// Created: SLG 2010-11-24
// -----------------------------------------------------------------------------
InhabitantHierarchies::InhabitantHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, superior )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationHierarchies destructor
// Created: SLG 2010-11-24
// -----------------------------------------------------------------------------
InhabitantHierarchies::~InhabitantHierarchies()
{
    if( superior_ )
        if( Inhabitants* popus = superior_->Retrieve< Inhabitants >() )
            popus->Remove( GetEntity().GetId() );
}
