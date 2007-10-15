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
IntelligencePrototype::IntelligencePrototype( Team_ABC& team, const std::string& symbol, const std::string& level, const Karma& karma )
    : team_  ( team )
    , symbol_( symbol )
    , level_ ( level )
    , karma_ ( karma )
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
    return factory.Create( team_, symbol_, level_, karma_, point );
}
