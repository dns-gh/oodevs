// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FormationLayer.h"
#include "ColorStrategy_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Formation_ABC >( controllers, tools, strategy, view, profile )
    , tools_   ( tools )
    , strategy_( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::~FormationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Draw
// Created: AGE 2007-06-14
// -----------------------------------------------------------------------------
void FormationLayer::Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport )
{
    const kernel::Formation_ABC& formation = static_cast< const kernel::Formation_ABC& >( entity );
    strategy_.SelectColor( formation );
    if( tools_.ShouldDisplay( "Formations" ) )
    {
//        viewport.SetHotpoint( position );
        entity.Draw( geometry::Point2f(), viewport, tools_ );
    }
}
