// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionAgentMission.h"
#include "SimulationMessages.h"
#include "AutomatDecisions.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionAgentMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionAgentMission::ActionAgentMission( const Entity_ABC& entity, const MissionType& mission, Controller& controller, bool registered /*= true*/ )
    : ActionMission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionAgentMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionAgentMission::ActionAgentMission( xml::xistream& xis, Controller& controller, const Resolver_ABC< MissionType >& missions, const kernel::Entity_ABC& entity )
    : ActionMission( xis, controller, missions, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionAgentMission destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionAgentMission::~ActionAgentMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionAgentMission::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionAgentMission::Publish( Publisher_ABC& publisher ) const
{
    // $$$$ SBO 2007-06-26: check profile! CanBeOrdered
    if( const Automat_ABC* automat = static_cast< const Automat_ABC* >( GetEntity().Get< kernel::TacticalHierarchies >().GetSuperior() ) )
    {
        const AutomatDecisions& decisions = automat->Get< AutomatDecisions >();
        if( decisions.IsEmbraye() )
            decisions.Disengage();
    }

    simulation::UnitOrder asn;
    asn().oid = GetEntity().GetId();
    asn().mission = GetType().GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}
