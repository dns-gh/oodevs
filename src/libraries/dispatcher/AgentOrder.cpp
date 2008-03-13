// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentOrder.h"
#include "Network_Def.h"
#include "Agent.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AgentOrder::AgentOrder( Model& model, Agent& agent, const ASN1T_MsgUnitOrder& asn )
    : Order_ABC( model, asn.mission, asn.parametres )
    , agent_   ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AgentOrder::~AgentOrder()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void AgentOrder::Send( ClientPublisher_ABC& publisher )
{
    AsnMsgSimToClientUnitOrder asn;
    asn().oid     = agent_.GetID();
    asn().mission = missionID_;
    Order_ABC::Send( asn().parametres );
    asn.Send( publisher );
    AsnDelete( asn().parametres );
}

// -----------------------------------------------------------------------------
// Name: AgentOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
void AgentOrder::SendNoMission( const Agent& agent, ClientPublisher_ABC& publisher )
{
    AsnMsgSimToClientUnitOrder asn;
    asn().oid          = agent.GetID();
    asn().mission      = 0;
    asn().parametres.n = 0;
    asn.Send( publisher );
}