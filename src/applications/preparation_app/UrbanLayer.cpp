// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanLayer.h"

// -----------------------------------------------------------------------------
// Name: UrbanLayer constructor
// Created: LGY 2012-01-06
// -----------------------------------------------------------------------------
UrbanLayer::UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                        gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter)
    : EditorProxy< gui::UrbanLayer >( controllers, tools, strategy, view, profile, filter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer destructor
// Created: LGY 2012-01-06
// -----------------------------------------------------------------------------
UrbanLayer::~UrbanLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::ShouldDisplay
// Created: LGY 2012-01-06
// -----------------------------------------------------------------------------
bool UrbanLayer::ShouldDisplay( const kernel::Entity_ABC& /*entity*/ )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::HandleMousePress
// Created: LGY 2012-01-16
// -----------------------------------------------------------------------------
bool UrbanLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( !livingAreaEditor_ )
        return EditorProxy< gui::UrbanLayer >::HandleMousePress( event, point );
    return false;
}
