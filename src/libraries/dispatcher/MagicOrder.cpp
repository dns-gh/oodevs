// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MagicOrder.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

MagicOrder::MagicOrder( const Model& /*model*/, const sword::MagicOrder& msg )
    : SimpleEntity<>( msg.id() )
    , order_( new sword::MagicOrder( msg ) )
{
    // NOTHING
}

MagicOrder::~MagicOrder()
{
    // NOTHING
}

void MagicOrder::DoUpdate( const sword::MagicOrder& msg )
{
    *order_ = msg;
}

void MagicOrder::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::MagicOrder msg;
    msg() = *order_;
    msg.Send( publisher );
}

void MagicOrder::SendFullUpdate( ClientPublisher_ABC& /*publisher */) const
{
    // NOTHING
}

void MagicOrder::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::MagicOrderDestruction msg;
    msg().set_id( order_->id() );
    msg.Send( publisher );
}

void MagicOrder::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}