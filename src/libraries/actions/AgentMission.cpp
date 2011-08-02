// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AgentMission.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace sword;
using namespace sword;
using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AgentMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AgentMission::AgentMission( const Entity_ABC& entity, const kernel::MissionType& mission, Controller& controller, bool registered /* = true*/ )
    : Mission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AgentMission::AgentMission( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub )
    : Mission( xis, controller, missions, entity, stub )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentMission destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AgentMission::~AgentMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentMission::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void AgentMission::Publish( Publisher_ABC& publisher, int ) const
{
    simulation::UnitOrder message;
    message().mutable_tasker()->set_id( GetEntity().GetId() );
    message().mutable_type()->set_id( GetType().GetId() );
    CommitTo( *message().mutable_parameters() );
    message.Send( publisher, 0 );
}
