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
#include "actions/ActionFactory.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/Point.h"
#include "actions/UnitMagicAction.h"
#include "gaming/ConvexHulls.h"
#include "gaming/MissionParameters.h"
#include "gaming/StaticModel.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/ValuedDragObject.h"
#include "protocol/simulationsenders.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AutomatsLayer constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
AutomatsLayer::AutomatsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const Profile_ABC& profile, gui::AgentsLayer& agents, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : gui::AutomatsLayer( controllers, tools, strategy, view, profile, agents )
    , tools_( tools )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , selected_( controllers )
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
void AutomatsLayer::NotifySelected( const Automat_ABC* automat )
{
    selected_ = automat;
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::HandleEnterDragEvent
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
bool AutomatsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
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
    kernel::Point location;
    location.AddPoint( point );

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "unit_creation" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Unit Creation" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), static_.coordinateConverter_, location ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatsLayer::RequestCreation
// Created: LDC 2010-10-01
// -----------------------------------------------------------------------------
void AutomatsLayer::RequestCreation( const geometry::Point2f& /*point*/, const kernel::AutomatType& type )
{
    Action_ABC* action = actionsModel_.CreateAutomatCreationAction( type , *selected_, controllers_.controller_, static_.types_ );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->Polish();
    actionsModel_.Publish( *action );
}
