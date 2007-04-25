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
AgentOrder::AgentOrder( Model& model, Agent& agent, const ASN1T_MsgPionOrder& asn )
    : Order_ABC( model, asn.mission, asn.parametres, asn.order_context )
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
void AgentOrder::Send( Publisher_ABC& publisher )
{
    AsnMsgSimToClientPionOrder asn;
    asn().oid_unite_executante = agent_.GetID();
    asn().mission              = missionID_;
    Order_ABC::Send( asn().order_context );
    Order_ABC::Send( asn().parametres );

    asn.Send( publisher );

    AsnDelete( asn().order_context );
    AsnDelete( asn().parametres );
}
