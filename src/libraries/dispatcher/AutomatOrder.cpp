// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AutomatOrder.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AutomatOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AutomatOrder::AutomatOrder( const Common::MsgAutomatOrder& message )
    : Order_ABC( message.type().id() )
    , message_( message.New() )
{
    message_->CopyFrom( message );
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AutomatOrder::~AutomatOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void AutomatOrder::Send( ClientPublisher_ABC& publisher ) const
{
    client::AutomatOrder message;
    message().CopyFrom( *message_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
void AutomatOrder::SendNoMission( const kernel::Entity_ABC& entity, ClientPublisher_ABC& publisher )
{
    client::AutomatOrder message;
    message().mutable_tasker()->set_id( entity.GetId() );
    message().mutable_type()->set_id( 0 );
    message.Send( publisher );
}
