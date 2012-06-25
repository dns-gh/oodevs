// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationKnowledgesLayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgesLayer constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
PopulationKnowledgesLayer::PopulationKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                                      View_ABC& view, const kernel::Profile_ABC& profile )
    : KnowledgeLayer< PopulationKnowledge_ABC >( controllers, tools, strategy, view, profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgesLayer destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
PopulationKnowledgesLayer::~PopulationKnowledgesLayer()
{
    // NOTHING
}
