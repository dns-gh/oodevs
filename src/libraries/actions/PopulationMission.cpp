// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PopulationMission.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/OrderType.h"
#include "protocol/SimulationSenders.h"
#include "protocol/Publisher_ABC.h"

using namespace Common;

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: PopulationMission constructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
PopulationMission::PopulationMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered /*= true*/ )
    : Mission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationMission constructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
PopulationMission::PopulationMission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity )
    : Mission( xis, controller, missions, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationMission destructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
PopulationMission::~PopulationMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationMission::Publish
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void PopulationMission::Publish( Publisher_ABC& publisher ) const
{
    simulation::PopulationOrder message;
    message().set_oid ( GetEntity().GetId() );
    message().set_mission ( GetType().GetId() );
    CommitTo( *message().mutable_parametres() );
    message.Send( publisher, 0 );
    Clean( *message().mutable_parametres() );
}

// -----------------------------------------------------------------------------
// Name: PopulationMission::Accept
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void PopulationMission::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
