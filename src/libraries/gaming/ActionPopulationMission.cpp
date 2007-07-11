// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionPopulationMission.h"
#include "ASN_Messages.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionPopulationMission constructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
ActionPopulationMission::ActionPopulationMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered /*= true*/ )
    : ActionMission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPopulationMission constructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
ActionPopulationMission::ActionPopulationMission( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity )
    : ActionMission( xis, controller, missions, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPopulationMission destructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
ActionPopulationMission::~ActionPopulationMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPopulationMission::Publish
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ActionPopulationMission::Publish( Publisher_ABC& publisher ) const
{
    ASN_MsgPopulationOrder asn;
    asn().oid = GetEntity().GetId();
    asn().mission = GetType().GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}
