// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationsLayerImp.h"
#include "gaming/Reports.h"
#include "gaming/Population.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationsLayerImp constructor
// Created: SBO 2006-08-21
// -----------------------------------------------------------------------------
PopulationsLayerImp::PopulationsLayerImp( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : PopulationsLayer( controllers, tools, strategy, view )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PopulationsLayerImp destructor
// Created: SBO 2006-08-21
// -----------------------------------------------------------------------------
PopulationsLayerImp::~PopulationsLayerImp()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayerImp::DisplayTooltip
// Created: SBO 2006-08-21
// -----------------------------------------------------------------------------
void PopulationsLayerImp::DisplayTooltip( const Population_ABC& entity, Displayer_ABC& displayer )
{
    const Population* popu = dynamic_cast< const Population* >( &entity ); // $$$$ AGE 2006-08-07: 
    if( popu )
        popu->DisplayInTooltip( displayer );
    if( const Reports* reports = entity.Retrieve< Reports >() )
        reports->DisplayInTooltip( displayer );
}
