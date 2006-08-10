// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledgesLayer.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgesLayer constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
PopulationKnowledgesLayer::PopulationKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : KnowledgeLayer< PopulationKnowledge >( controllers, tools, strategy, view )
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
