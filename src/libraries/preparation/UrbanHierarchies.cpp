// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies constructor
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
UrbanHierarchies::UrbanHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
    : EntityHierarchies< Hierarchies >( controller, entity, superior )
{
    if( superior )
    {
        const kernel::Hierarchies* superiorHierarchies = RetrieveHierarchies( *superior );
        level_ = ( superiorHierarchies && superiorHierarchies->GetSuperior() ) ? eLevelBlock : eLevelDistrict;
    }
    else 
        level_ = eLevelCity;
}

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies destructor
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
UrbanHierarchies::~UrbanHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies::GetLevel
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
UrbanHierarchies::ELevel UrbanHierarchies::GetLevel() const
{
    return level_;
}
