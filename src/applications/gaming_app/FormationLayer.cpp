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
#include "actions/AutomatCreationListener.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "clients_gui/ValuedDragObject.h"
#include "clients_kernel/PopulationPrototype.h"
#include "gaming/AgentServerMsgMgr.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"
#include <ctime>

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                                gui::View_ABC& view, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel,
                                const StaticModel& staticModel, const kernel::Time_ABC& simulation, AgentServerMsgMgr& messageManager,
                                tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, const gui::LayerFilter_ABC& filter )
    : gui::FormationLayer( controllers, tools, strategy, view, profile, filter )
    , selected_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , messageManager_( messageManager )
    , agentsModel_( agentsModel )
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
    return selected_ && ( gui::ValuedDragObject::Provides< const kernel::AutomatType >( event )
                         || gui::ValuedDragObject::Provides< const kernel::PopulationPrototype >( event ) );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::HandleDropEvent
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
bool FormationLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( selected_ )
    {
        if( const kernel::AutomatType* droppedItem = gui::ValuedDragObject::GetValue< const kernel::AutomatType >( event ) )
        {
            RequestCreation( point, *droppedItem );
            return true;
        }
        if( const kernel::PopulationPrototype* droppedItem = gui::ValuedDragObject::GetValue< const kernel::PopulationPrototype >( event ) )
        {
            RequestCreation( point, *droppedItem );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifySelected
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
void FormationLayer::NotifySelected( const kernel::Formation_ABC* formation )
{
    selected_ = formation;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::RequestCreation
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
void FormationLayer::RequestCreation( const geometry::Point2f& point, const kernel::AutomatType& type )
{
    actions::Action_ABC* action = actionsModel_.CreateAutomatCreationAction( point, type, *selected_, agentsModel_, messageManager_, simulation_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->Polish();
    int context = (int)clock();
//    boost::shared_ptr< sword::Listener > listener( new AutomatCreationListener( point, type, context,
//        agentsModel_, controllers_.controller_, actionsModel_, simulation_ ) );
//    messageManager_.RegisterListener( listener );
    actionsModel_.Publish( *action, context );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::RequestCreation
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
void FormationLayer::RequestCreation( const geometry::Point2f& point, const kernel::PopulationPrototype& type )
{
    actions::Action_ABC* action = actionsModel_.CreateCrowdCreationAction( *(type.type_), type.number_, point, *selected_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->Polish();
    actionsModel_.Publish( *action );
}
