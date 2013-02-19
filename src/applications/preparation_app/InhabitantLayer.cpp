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
                                  LivingAreaEditor_ABC& editor )
    : gui::InhabitantLayer( controllers, tools, strategy, view, profile )
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
    if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() != ePreparationMode_LivingArea )
        gui::InhabitantLayer::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::HandleMousePress
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
bool InhabitantLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() != ePreparationMode_LivingArea )
        return false;
    if( event && event->buttons() != Qt::NoButton )
    {
        if( event->button() == Qt::LeftButton )
            editor_.Select( point );
        if( event->button() == Qt::RightButton )
            controllers_.actions_.ContextMenu( point, kernel::Nothing(), event->globalPos() );
    }
    return true;
}
