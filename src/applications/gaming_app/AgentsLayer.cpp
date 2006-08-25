// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentsLayer.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Displayer_ABC.h"
#include "gaming/Reports.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view )
    : gui::AgentsLayer( controllers, tools, strategy, view )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: AgentsLayer::AddToTooltip
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename Extension >
void AgentsLayer::AddToTooltip( const Agent_ABC& entity, Displayer_ABC& displayer )
{
    if( const Extension* extension = entity.Retrieve< Extension >() )
        extension->DisplayInTooltip( displayer );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::DisplayTooltip
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentsLayer::DisplayTooltip( const Agent_ABC& entity, Displayer_ABC& displayer )
{
    displayer.Display( "", entity );
    // $$$$ AGE 2006-08-24: Apply Displayable_ABC::DisplayInTooltip
    AddToTooltip< Attributes_ABC >  ( entity, displayer );
    AddToTooltip< Decisions >       ( entity, displayer );
    AddToTooltip< AutomatDecisions >( entity, displayer );
    AddToTooltip< Reports >         ( entity, displayer );
}
