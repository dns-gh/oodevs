// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AutomatsLayer.h"
#include "actions/AutomatCreationListener.h"
#include "actions/ActionFactory.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/Point.h"
#include "actions/UnitMagicAction.h"
#include "gaming/ConvexHulls.h"
#include "gaming/MissionParameters.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/ValuedDragObject.h"
#include "gaming/AgentServerMsgMgr.h"
#include "protocol/SimulationSenders.h"
#include <ctime>

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view,
                             const Profile_ABC& profile, actions::ActionsModel& actionsModel,
                             const kernel::Time_ABC& simulation, AgentServerMsgMgr& messageManager,
                             tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, const gui::LayerFilter_ABC& filter )
    : gui::AutomatsLayer( controllers, tools, strategy, view, profile, filter )
    , tools_         ( tools )
    , actionsModel_  ( actionsModel )
    , simulation_    ( simulation )
    , selected_      ( controllers )
    , messageManager_( messageManager )
    , agentsModel_   ( agentsModel )
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

// $$$$ AGE 2007-06-14: pas super terrible tout ca

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AutomatsLayer::Draw( const Entity_ABC& entity, Viewport_ABC& viewport )
{
    gui::AutomatsLayer::Draw( entity, viewport );
    if( !ShouldDisplay( entity ) && selected_ == &entity )
    {
        SelectColor( entity );
        const Positions& positions = entity.Get< Positions >();
        const geometry::Point2f position = positions.GetPosition();
        viewport.SetHotpoint( position );
        selected_->Get< MissionParameters >().Draw( position, viewport, tools_ );
        selected_->Get< ConvexHulls >().Draw( position, viewport, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::NotifySelected
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AutomatsLayer::NotifySelected( const kernel::Automat_ABC* automat )
{
    selected_ = automat;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleMousePress
// Created: JSR 2011-06-01
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool ret = gui::AutomatsLayer::HandleMousePress( event, point );
    if( !ret && selected_
        && event && event->button() == Qt::LeftButton && ( event->state() & Qt::ShiftButton ) )
        return true;
    return ret;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::CanDrop
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    return selected_ && ( gui::ValuedDragObject::Provides< const AgentType >( event )
        || gui::ValuedDragObject::Provides< const AutomatType >( event ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleDropEvent
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( !selected_ )
        return false;
    if( const AgentType* droppedItem = gui::ValuedDragObject::GetValue< const AgentType >( event ) )
    {
        RequestCreation( point, *droppedItem );
        return true;
    }
    if( const AutomatType* droppedItem = gui::ValuedDragObject::GetValue< const AutomatType >( event ) )
    {
        RequestCreation( point, *droppedItem );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::RequestCreation
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AutomatsLayer::RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type )
{
    actions::Action_ABC* action = actionsModel_.CreateAgentCreationAction( type, point, *selected_ );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->Polish();
    actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::RequestCreation
// Created: LDC 2010-10-01
// -----------------------------------------------------------------------------
void AutomatsLayer::RequestCreation( const geometry::Point2f& point, const kernel::AutomatType& type )
{
    Action_ABC* action = actionsModel_.CreateAutomatCreationAction( point, type, *selected_, agentsModel_, messageManager_, simulation_ );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->Polish();
    int context = (int)clock();
    actionsModel_.Publish( *action, context );
}
