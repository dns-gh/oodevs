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
#include "ClientPublisher_ABC.h"
#include "Agent.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AgentOrder::AgentOrder( Model_ABC& model, Agent& agent, const Common::MsgUnitOrder& asn )
    : Order_ABC( model, asn.mission(), asn.parametres() )
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
    client::UnitOrder asn;
    asn().set_oid    ( agent_.GetId() );
    asn().set_mission( missionID_ );
    Order_ABC::Send( *asn().mutable_parametres() );
    asn.Send( publisher );
    Delete( *asn().mutable_parametres() );
}

// -----------------------------------------------------------------------------
// Name: AgentOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
void AgentOrder::SendNoMission( const Agent& agent, ClientPublisher_ABC& publisher )
{
    client::UnitOrder asn;
    asn().set_oid          ( agent.GetId() );
    asn().set_mission      ( 0 );
    asn.Send( publisher );
}