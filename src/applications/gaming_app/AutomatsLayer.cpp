// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AutomatsLayer.h"
#include "actions/ActionsModel.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/Viewport_ABC.h"
#include "gaming/ConvexHulls.h"
#include "gaming/MissionParameters.h"

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( kernel::Controllers& controllers,
                              gui::GLView_ABC& view,
                              gui::ColorStrategy_ABC& strategy,
                              const kernel::Profile_ABC& profile,
                              actions::ActionsModel& actionsModel )
    : gui::AutomatsLayer( controllers, view, strategy, profile )
    , actionsModel_( actionsModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
AutomatsLayer::~AutomatsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AutomatsLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool pickingMode )
{
    gui::AutomatsLayer::Draw( entity, viewport, pickingMode );
    if( selected_ == &entity && !ShouldDisplay( entity ) )
    {
        SelectColor( entity );
        const kernel::Positions& positions = entity.Get< kernel::Positions >();
        const geometry::Point2f position = positions.GetPosition();
        viewport.SetHotpoint( position );
        selected_->Get< MissionParameters >().Draw( position, viewport, view_ );
        selected_->Get< ConvexHulls >().Draw( position, viewport, view_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleMousePress
// Created: JSR 2011-06-01
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool ret = gui::AutomatsLayer::HandleMousePress( event, point );
    if( !ret && selected_
        && event && event->button() == Qt::LeftButton && ( event->modifiers() & Qt::ShiftModifier ) )
        return true;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::CanDrop
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    return selected_ && ( dnd::HasData< const kernel::AgentType >( event ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleDropEvent
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( !selected_ )
        return false;
    if( const kernel::AgentType* droppedItem = dnd::FindData< kernel::AgentType >( event ) )
    {
        actionsModel_.PublishAgentCreationAction( *droppedItem, point, *selected_ );
        return true;
    }
    return false;
}
