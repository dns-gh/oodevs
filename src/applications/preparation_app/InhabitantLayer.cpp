// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InhabitantLayer.h"

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::InhabitantLayer
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
InhabitantLayer::InhabitantLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools,
                                  gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile,
                                  const gui::LayerFilter_ABC& filter )
    : EditorProxy< gui::InhabitantLayer >( controllers, tools, strategy, view, profile, filter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::~InhabitantLayer
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
InhabitantLayer::~InhabitantLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::ShouldDisplay
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
bool InhabitantLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    return gui::InhabitantLayer::ShouldDisplay( entity );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::NotifySelected
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
void InhabitantLayer::NotifySelected( const kernel::Inhabitant_ABC* inhabitant )
{
    if( !livingAreaEditor_ )
        EditorProxy< gui::InhabitantLayer >::NotifySelected( inhabitant );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::HandleMousePress
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
bool InhabitantLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( !livingAreaEditor_ )
        return EditorProxy< gui::InhabitantLayer >::HandleMousePress( event, point );
    if( event && event->state() != Qt::NoButton )
        if( event->button() == Qt::RightButton )
            controllers_.actions_.ContextMenu( point, kernel::Nothing(), event->globalPos() );
    return true;
}
