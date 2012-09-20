// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AutomatMission.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace sword;
using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AutomatMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AutomatMission::AutomatMission( const Entity_ABC& entity, const kernel::MissionType& mission, Controller& controller, bool registered /* = true*/ )
    : Mission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AutomatMission::AutomatMission( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub )
    : Mission( xis, controller, missions, entity, stub )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatMission destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AutomatMission::~AutomatMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatMission::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void AutomatMission::Publish( Publisher_ABC& publisher, int ) const
{
    simulation::AutomatOrder message;
    message().mutable_tasker()->set_id( GetEntity().GetId());
    message().mutable_type()->set_id( GetType().GetId());
    CommitTo( *message().mutable_parameters() );
    message.Send( publisher );
}
