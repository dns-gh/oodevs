// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TeamLayer.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/PopulationPrototype.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/AgentServerMsgMgr.h"

// -----------------------------------------------------------------------------
// Name: TeamLayer constructor
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
TeamLayer::TeamLayer( kernel::Controllers& controllers,
                      gui::GLView_ABC& view,
                      gui::ColorStrategy_ABC& strategy,
                      const kernel::Profile_ABC& profile,
                      actions::ActionsModel& actionsModel,
                      const StaticModel& staticModel,
                     const kernel::Time_ABC& simulation,
                     AgentServerMsgMgr& messageManager )
    : gui::EntityLayer< kernel::Team_ABC >( controllers, view, strategy, profile, eLayerTypes_Parties )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , messageManager_( messageManager )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer destructor
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
TeamLayer::~TeamLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::CanDrop
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    return selected_ && dnd::HasData< kernel::PopulationPrototype >( event );
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::HandleDropEvent
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( selected_ )
    {
        if( const kernel::PopulationPrototype* type = dnd::FindData< kernel::PopulationPrototype >( event ) )
        {
            actionsModel_.PublishCrowdCreationAction( *type->type_, type->number_, 0, 0, point, *selected_ );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::HandleMousePress
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleMousePress( QMouseEvent* , const geometry::Point2f& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::HandleMouseMove
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleMouseMove( QMouseEvent* , const geometry::Point2f& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void TeamLayer::NotifySelectionChanged( const std::vector< const kernel::Team_ABC* >& elements )
{
    selected_ = elements.size() == 1 ? elements.front() : 0;
    gui::EntityLayer< kernel::Team_ABC >::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::NotifyActivated
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
void TeamLayer::NotifyActivated( const kernel::Team_ABC& )
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::Paint
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::Paint( const ViewFrustum& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::Paint
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::Paint( const geometry::Rectangle2f& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::Paint
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::Paint( gui::Viewport_ABC& )
{
    // NOTHING
}
