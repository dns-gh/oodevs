// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationsLayer.h"
#include "gaming/Reports.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/Population.h"

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayer< Population_ABC >( controllers, tools, strategy, view )
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

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void PopulationsLayer::DisplayTooltip( const Population_ABC& entity, Displayer_ABC& displayer )
{
    const Population* popu = dynamic_cast< const Population* >( &entity ); // $$$$ AGE 2006-08-07: 
    if( popu )
        popu->DisplayInTooltip( displayer );
    if( const Reports* reports = entity.Retrieve< Reports >() )
        reports->DisplayInTooltip( displayer );
}            
