// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentsLayerImp.h"
#include "gaming/Attributes.h"
#include "gaming/Reports.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"
#include "clients_kernel/Displayer_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentsLayerImp constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentsLayerImp::AgentsLayerImp( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : AgentsLayer( controllers, tools, strategy, view )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayerImp destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
AgentsLayerImp::~AgentsLayerImp()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: AgentsLayer::AddToTooltip
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename Extension >
void AgentsLayerImp::AddToTooltip( const Agent_ABC& entity, Displayer_ABC& displayer )
{
    if( const Extension* extension = entity.Retrieve< Extension >() )
        extension->DisplayInTooltip( displayer );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayerImp::DisplayTooltip
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void AgentsLayerImp::DisplayTooltip( const Agent_ABC& entity, Displayer_ABC& displayer )
{
    displayer.Display( "", entity );
    AddToTooltip< Attributes >      ( entity, displayer );
    AddToTooltip< Decisions >       ( entity, displayer );
    AddToTooltip< AutomatDecisions >( entity, displayer );
    AddToTooltip< Reports >         ( entity, displayer );
}
