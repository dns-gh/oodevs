// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationsLayer.h"

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, MapWidget& widget )
    : EntityLayer< Population >( controllers, tools, strategy, widget )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationsLayer::~PopulationsLayer()
{
    // NOTHING
}
