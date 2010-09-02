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
#include "Agent_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AgentOrder::AgentOrder( const Common::MsgUnitOrder& message )
    : Order_ABC( message.type().id() )
    , message_( message.New() )
{
    message_->CopyFrom( message );
}

// -----------------------------------------------------------------------------
// Name: AgentOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AgentOrder::~AgentOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void AgentOrder::Send( ClientPublisher_ABC& publisher ) const
{
    client::UnitOrder message;
    message().CopyFrom( *message_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: AgentOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
void AgentOrder::SendNoMission( const kernel::Entity_ABC& entity, ClientPublisher_ABC& publisher )
{
    client::UnitOrder message;
    message().mutable_tasker()->set_id( entity.GetId() );
    message().mutable_type()->set_id( 0 );
    message.Send( publisher );
}