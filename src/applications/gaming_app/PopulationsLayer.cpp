// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationsLayer.h"
#include "gaming/Reports.h"
#include "gaming/Population.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: SBO 2006-08-21
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                                    gui::View_ABC& view, const Profile_ABC& profile )
    : gui::PopulationsLayer( controllers, tools, strategy, view, profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer destructor
// Created: SBO 2006-08-21
// -----------------------------------------------------------------------------
PopulationsLayer::~PopulationsLayer()
{
    // NOTHING
}
