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
#include "clients_kernel/ModeController_ABC.h"

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::InhabitantLayer
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
InhabitantLayer::InhabitantLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools,
                                  gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile,
                                  const gui::LayerFilter_ABC& filter, LivingAreaEditor_ABC& editor )
    : gui::InhabitantLayer( controllers, tools, strategy, view, profile, filter )
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
// Name: InhabitantLayer::NotifySelected
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
void InhabitantLayer::NotifySelected( const kernel::Inhabitant_ABC* inhabitant )
{
    if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() != ePreparationMode_LivingArea )
        gui::InhabitantLayer::NotifySelected( inhabitant );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::HandleMousePress
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
bool InhabitantLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() != ePreparationMode_LivingArea )
        return gui::InhabitantLayer::HandleMousePress( event, point );
    if( event && event->state() != Qt::NoButton )
    {
        if( event->button() == Qt::LeftButton )
            editor_.Select( point );
        if( event->button() == Qt::RightButton )
            controllers_.actions_.ContextMenu( point, kernel::Nothing(), event->globalPos() );
    }
    return true;
}
