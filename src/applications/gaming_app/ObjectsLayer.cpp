// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectsLayer.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ObjectMagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                            gui::View_ABC& view, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel,
                            const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::TerrainPicker& picker )
    : gui::ObjectsLayer( controllers, tools, strategy, view, profile, picker )
    , actionsModel_( actionsModel )
    , static_      ( staticModel )
    , simulation_  ( simulation )
    , selected_    ( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleKeyPress
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "destroy_object" );
        std::unique_ptr< Action_ABC > action( new ObjectMagicAction( actionType, controllers_.controller_, false ) );
        action->Rename( tools::translate( "gaming_app::Action", "Object Destruction" ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        actionsModel_.Publish( *action );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifySelectionChanged( const std::vector< const kernel::Object_ABC* >& elements )
{
    selected_ = elements.size() == 1 ? elements.front() : 0;
    gui::ObjectsLayer::NotifySelectionChanged( elements );
}
