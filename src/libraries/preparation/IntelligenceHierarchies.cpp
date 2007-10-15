// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IntelligenceHierarchies.h"
#include "Intelligences.h"

// -----------------------------------------------------------------------------
// Name: IntelligenceHierarchies constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligenceHierarchies::IntelligenceHierarchies( kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::SimpleHierarchies< kernel::TacticalHierarchies >( holder, superior )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceHierarchies destructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligenceHierarchies::~IntelligenceHierarchies()
{
    if( superior_ )
        if( Intelligences* intelligences = superior_->Retrieve< Intelligences >() )
            intelligences->Remove( GetEntity().GetId() );
}
