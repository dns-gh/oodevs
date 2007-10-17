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
#include "clients_kernel/Intelligence_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligenceHierarchies constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligenceHierarchies::IntelligenceHierarchies( Intelligence_ABC& holder, Entity_ABC* superior )
    : SimpleHierarchies< TacticalHierarchies >( holder, superior )
    , concreteEntity_( holder )
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

// -----------------------------------------------------------------------------
// Name: IntelligenceHierarchies::GetSymbol
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
std::string IntelligenceHierarchies::GetSymbol() const
{
    return concreteEntity_.GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: IntelligenceHierarchies::GetLevel
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
std::string IntelligenceHierarchies::GetLevel() const
{
    return concreteEntity_.GetLevel();
}
