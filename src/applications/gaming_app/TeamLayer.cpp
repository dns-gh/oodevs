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
#include "clients_gui/ValuedDragObject.h"
#include "clients_kernel/PopulationPrototype.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/StaticModel.h"
#include "protocol/simulationsenders.h"

// -----------------------------------------------------------------------------
// Name: TeamLayer constructor
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
TeamLayer::TeamLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view,
                     const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel, const StaticModel& staticModel,
                     const kernel::Time_ABC& simulation, AgentServerMsgMgr& messageManager )
: gui::EntityLayer< kernel::Team_ABC >( controllers, tools, strategy, view, profile )
, actionsModel_( actionsModel )
, static_( staticModel )
, simulation_( simulation )
, messageManager_( messageManager )
, selected_( controllers, 0 )
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
// Name: TeamLayer::HandleEnterDragEvent
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    return selected_ && gui::ValuedDragObject::Provides< const kernel::PopulationPrototype >( event );
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::HandleDropEvent
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( selected_ )
    {
        if( const kernel::PopulationPrototype* type = gui::ValuedDragObject::GetValue< const kernel::PopulationPrototype >( event ) )
        {
            actions::Action_ABC* action = actionsModel_.CreateCrowdCreationAction( *(type->type_), type->number_, point, *selected_, controllers_.controller_, static_.types_, static_.coordinateConverter_ );
            action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new actions::ActionTasker( selected_, false ) );
            action->Polish();
            actionsModel_.Publish( *action );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::HandleMousePress
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::HandleMouseMove
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
bool TeamLayer::HandleMouseMove( QMouseEvent* , const geometry::Point2f& point )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::NotifySelected
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::NotifySelected( const kernel::Team_ABC* team )
{
    selected_ = team;
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::Paint
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::Paint( const ViewFrustum& frustum )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::Paint
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::Paint( const geometry::Rectangle2f& viewport )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamLayer::Paint
// Created: LDC 2010-10-26
// -----------------------------------------------------------------------------
void TeamLayer::Paint( kernel::Viewport_ABC& viewport )
{
    // NOTHING
}
