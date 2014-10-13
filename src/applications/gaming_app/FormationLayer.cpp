// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FormationLayer.h"
#include "actions/ActionsModel.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/PopulationPrototype.h"
#include <ctime>

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers,
                                gui::GLView_ABC& view,
                                gui::ColorStrategy_ABC& strategy,
                                const kernel::Profile_ABC& profile,
                                actions::ActionsModel& actionsModel,
                                const StaticModel& staticModel )
    : gui::FormationLayer( controllers, view, strategy, profile )
    , selected_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::~FormationLayer
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
FormationLayer::~FormationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::CanDrop
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
bool FormationLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& /*point*/ ) const
{
    return selected_ && ( dnd::HasData< const kernel::AutomatType >( event ) || dnd::HasData< const kernel::PopulationPrototype >( event ) );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::HandleDropEvent
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
bool FormationLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( selected_ )
    {
        if( const kernel::AutomatType* droppedItem = dnd::FindData< kernel::AutomatType >( event ) )
        {
            RequestCreation( point, *droppedItem );
            return true;
        }
        if( const kernel::PopulationPrototype* droppedItem = dnd::FindData< kernel::PopulationPrototype >( event ) )
        {
            RequestCreation( point, *droppedItem );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void FormationLayer::NotifySelectionChanged( const std::vector< const kernel::Formation_ABC* >& elements )
{
    selected_ = elements.size() == 1 ? elements.front() : 0;
    gui::FormationLayer::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::RequestCreation
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
void FormationLayer::RequestCreation( const geometry::Point2f& point, const kernel::AutomatType& type )
{
    actionsModel_.PublishAutomatCreationAction( point, type, *selected_ );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::RequestCreation
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
void FormationLayer::RequestCreation( const geometry::Point2f& point, const kernel::PopulationPrototype& type )
{
    actionsModel_.PublishCrowdCreationAction( *(type.type_), type.number_, 0, 0, point, *selected_ );
}
