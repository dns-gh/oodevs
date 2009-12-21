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
#include "game_asn/SimulationSenders.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: AgentMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AgentMission::AgentMission( const Entity_ABC& entity, const MissionType& mission, Controller& controller, bool registered /*= true*/ )
    : Mission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
AgentMission::AgentMission( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< MissionType >& missions, const kernel::Entity_ABC& entity )
    : Mission( xis, controller, missions, entity )
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
void AgentMission::Publish( Publisher_ABC& publisher ) const
{
    simulation::UnitOrder asn;
    asn().oid = GetEntity().GetId();
    asn().mission = GetType().GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}

// -----------------------------------------------------------------------------
// Name: AgentMission::Accept
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void AgentMission::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
