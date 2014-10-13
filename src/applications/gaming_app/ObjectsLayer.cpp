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
#include "clients_kernel/Profile_ABC.h"
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
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers,
                            gui::GLView_ABC& view,
                            gui::ColorStrategy_ABC& strategy,
                            const kernel::Profile_ABC& profile,
                            actions::ActionsModel& actionsModel,
                            const ::StaticModel& staticModel,
                            const kernel::Time_ABC& simulation,
                            gui::TerrainPicker& picker )
    : gui::ObjectsLayer( controllers, view, strategy, profile, picker )
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
    if( selected_ && profile_.CanDoMagic( *selected_ ) && key->key() == Qt::Key_Delete )
        actionsModel_.PublishObjectDestroyMagicAction( *selected_ );
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
