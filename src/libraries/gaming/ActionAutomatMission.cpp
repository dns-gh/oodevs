// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionAutomatMission.h"
#include "gaming/ASN_Messages.h"
#include "clients_kernel/OrderType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionAutomatMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionAutomatMission::ActionAutomatMission( const Entity_ABC& entity, const MissionType& mission, Controller& controller, const Simulation& simulation, bool registered /*= true*/ )
    : ActionMission( entity, mission, controller, simulation, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionAutomatMission constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionAutomatMission::ActionAutomatMission( xml::xistream& xis, Controller& controller, const Resolver_ABC< MissionType >& missions, const Entity_ABC& entity, const Simulation& simulation )
    : ActionMission( xis, controller, missions, entity, simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionAutomatMission destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionAutomatMission::~ActionAutomatMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionAutomatMission::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionAutomatMission::Publish( Publisher_ABC& publisher ) const
{
    ASN_MsgAutomatOrder asn;
    asn().oid_unite_executante = GetEntity().GetId();
    asn().mission = GetType().GetId();
    asn().formation = (ASN1T_EnumAutomatOrderFormation)EnumAutomatOrderFormation::deux_echelons; // $$$$ SBO 2007-03-15: move to order.parameters
    CommitTo( asn().order_context );
    CommitTo( asn().parametres );
    asn.Send( publisher );
    Clean( asn().order_context );
    Clean( asn().parametres );
}
