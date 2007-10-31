// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "IntelligencesLayer.h"
#include "gaming/IntelligencePositions.h"
#include "clients_kernel/Intelligence_ABC.h"

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer constructor
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
IntelligencesLayer::IntelligencesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, kernel::IntelligenceFactory_ABC& factory )
    : gui::IntelligencesLayer( controllers, tools, strategy, view, profile, factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer destructor
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
IntelligencesLayer::~IntelligencesLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::Delete
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void IntelligencesLayer::Delete( const kernel::Intelligence_ABC& element )
{
    const_cast< kernel::Intelligence_ABC& >( element ).Delete();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::Move
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void IntelligencesLayer::Move( kernel::Positions& position, const geometry::Point2f& point )
{
    static_cast< IntelligencePositions& >( position ).Set( point );
}
