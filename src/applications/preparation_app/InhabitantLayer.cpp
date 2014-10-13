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
#include "LivingAreaEditor_ABC.h"

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::InhabitantLayer
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
InhabitantLayer::InhabitantLayer( kernel::Controllers& controllers,
                                  gui::GLView_ABC& view,
                                  gui::ColorStrategy_ABC& strategy,
                                  const kernel::Profile_ABC& profile,
                                  LivingAreaEditor_ABC& editor )
    : gui::InhabitantLayer( controllers, view, strategy, profile )
    , editor_( editor )
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
// Name: InhabitantLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void InhabitantLayer::NotifySelectionChanged( const std::vector< const kernel::Inhabitant_ABC* >& elements )
{
    if( controllers_.GetCurrentMode() != eModes_LivingArea )
        gui::InhabitantLayer::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::HandleMousePress
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
bool InhabitantLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( controllers_.GetCurrentMode() != eModes_LivingArea )
        return false;
    if( event && event->buttons() != Qt::NoButton )
    {
        if( event->button() == Qt::LeftButton )
            editor_.Select( point );
        if( event->button() == Qt::RightButton )
            controllers_.actions_.ContextMenu( this, point, kernel::Nothing(), event->globalPos() );
    }
    return true;
}
