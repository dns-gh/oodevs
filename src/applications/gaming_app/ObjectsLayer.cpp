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
#include "actions/ObjectMagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Object_ABC.h"
#include "gaming/ActionPublisher.h"
#include "gaming/ActionTiming.h"
#include "gaming/StaticModel.h"
#include "protocol/simulationsenders.h"
#include "protocol/clientsenders.h"

using namespace Common;
using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation )
    : gui::ObjectsLayer( controllers, tools, strategy, view, profile )
    , actionPublisher_( actionPublisher )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , selected_( controllers )
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
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "destroy_object" );
        ObjectMagicAction* action = new ObjectMagicAction( selected_, actionType, controllers_.controller_, true );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->RegisterAndPublish( actionsModel_, actionPublisher_ );

        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifySelected
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifySelected( const kernel::Object_ABC* object )
{
    selected_ = object;
    gui::ObjectsLayer::NotifySelected( object );
}
