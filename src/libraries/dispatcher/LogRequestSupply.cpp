// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogRequestSupply.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogRequestSupply constructor
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
LogRequestSupply::LogRequestSupply( const Model& /*model*/, const sword::LogSupplyRequestCreation& msg )
    : SimpleEntity< >( msg.request().id() )
    , nDotationType_ ( msg.resource().id() )
    , requester_     ( msg.requester().id() )
    , nState_        ( sword::LogSupplyRequestUpdate::request_outstanding )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogRequestSupply destructor
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
LogRequestSupply::~LogRequestSupply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogRequestSupply::Update
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void LogRequestSupply::Update( const sword::LogSupplyRequestUpdate& msg )
{
    nState_ = msg.state();
    recipient_ = msg.recipient().id();
    supplier_ = msg.supplier().id();
    requested_ = msg.requested();
    granted_ = msg.granted();
    convoyed_ = msg.convoyed();
    delivered_ = msg.delivered();
}

// -----------------------------------------------------------------------------
// Name: LogRequestSupply::SendCreation
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void LogRequestSupply::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyRequestCreation msg;
    msg().mutable_request()->set_id( GetId() );
    msg().mutable_requester()->set_id( requester_ );
    msg().mutable_resource()->set_id( nDotationType_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogRequestSupply::SendFullUpdate
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void LogRequestSupply::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyRequestUpdate msg;
    msg().mutable_request()->set_id( GetId() );
    msg().set_state( nState_ );
    msg().mutable_recipient()->set_id( recipient_ );
    msg().mutable_supplier()->set_id( supplier_ );
    msg().set_requested( requested_ );
    msg().set_granted( granted_ );
    msg().set_convoyed( requested_ );
    msg().set_delivered( requested_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogRequestSupply::SendDestruction
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void LogRequestSupply::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyRequestDestruction msg;
    msg().mutable_request()->set_id( GetId() );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LogRequestSupply::Accept
// Created: LGY 2014-04-30
// -----------------------------------------------------------------------------
void LogRequestSupply::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
