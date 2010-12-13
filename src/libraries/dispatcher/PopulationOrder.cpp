// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationOrder.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
PopulationOrder::PopulationOrder( const sword::CrowdOrder& message )
    : Order_ABC( message.type().id() )
    , message_( message.New() )
{
    message_->CopyFrom( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
PopulationOrder::~PopulationOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void PopulationOrder::Send( ClientPublisher_ABC& publisher ) const
{
    client::CrowdOrder message;
    message().CopyFrom( *message_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
void PopulationOrder::SendNoMission( const kernel::Entity_ABC& entity, ClientPublisher_ABC& publisher )
{
    client::CrowdOrder message;
    message().mutable_tasker()->set_id( entity.GetId() );
    message().mutable_type()->set_id( 0 );
    message.Send( publisher );
}
