// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "IntelligencePrototype.h"
#include "IntelligenceFactory_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencePrototype constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencePrototype::IntelligencePrototype( Entity_ABC& superior, const std::string& symbol, const HierarchyLevel_ABC& level, bool embarked, const Karma& karma )
    : superior_( superior )
    , symbol_  ( symbol )
    , level_   ( level )
    , embarked_( embarked )
    , karma_   ( karma )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencePrototype destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencePrototype::~IntelligencePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencePrototype::CreateIntelligence
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence_ABC& IntelligencePrototype::CreateIntelligence( IntelligenceFactory_ABC& factory, const geometry::Point2f& point ) const
{
    return *factory.Create( superior_, symbol_, level_, embarked_, karma_, point );
}
