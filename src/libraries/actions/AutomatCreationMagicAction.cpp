// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "AutomatCreationMagicAction.h"
#include "AutomatCreationListener.h"
#include "ActionsModel.h"
#include "Point.h"
#include "CreationListener_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/ServerPublisher_ABC.h"
#include <time.h>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: AutomatCreationMagicAction constructor
// Created: HBD 2010-12-07
// -----------------------------------------------------------------------------
AutomatCreationMagicAction::AutomatCreationMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller,
                                                       const QString& name, const kernel::StaticModel& staticModel, const kernel::AutomatType& type, const geometry::Point2f& point,
                                                       tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, actions::CreationListener_ABC& agentMessenger, ActionsModel& actionsModel,
                                                       const kernel::Time_ABC& simulation, bool registered /* = true*/)
    : UnitMagicAction( entity, magic, controller, name, registered )
    , automatType_ ( type )
    , coordinateConverter_ ( staticModel.coordinateConverter_ )
    , agentsModel_ ( agentsModel )
    , messageManager_ ( agentMessenger )
    , actionsModel_ ( actionsModel )
    , agentTypes_ ( staticModel.types_ )
    , simulation_ ( simulation )
    , point_ ( point )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCreationMagicAction destructor
// Created: HBD 2010-12-06
// -----------------------------------------------------------------------------
AutomatCreationMagicAction::~AutomatCreationMagicAction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCreationMagicAction::Publish
// Created: HBD 2010-12-06
// -----------------------------------------------------------------------------
void AutomatCreationMagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
   boost::shared_ptr< sword::Listener > listener( new AutomatCreationListener( point_, automatType_, context,
            agentsModel_, controller_, actionsModel_, simulation_ ) );
    messageManager_.RegisterListener( listener );
    UnitMagicAction::Publish( publisher, context );
}
