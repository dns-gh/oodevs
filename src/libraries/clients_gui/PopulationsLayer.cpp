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

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                    View_ABC& view, const Profile_ABC& profile )
    : EntityLayer< Population_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Crowds )
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
// Name: PopulationsLayer::ContextMenu
// Created: SBO 2006-11-28
// -----------------------------------------------------------------------------
void PopulationsLayer::ContextMenu( const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Population_ABC& popu = static_cast< const Population_ABC& >( entity );
    controllers_.actions_.ContextMenu( this, popu, entity, point, where );
}
