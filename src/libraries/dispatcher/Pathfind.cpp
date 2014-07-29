// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Pathfind.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

Pathfind::Pathfind( const Model& /*model*/, const sword::Pathfind& msg )
    : SimpleEntity<>( msg.id() )
    , data_( new sword::Pathfind( msg ) )
{
    // NOTHING
}

Pathfind::~Pathfind()
{
    // NOTHING
}

void Pathfind::DoUpdate( const sword::Pathfind& msg )
{
    *data_ = msg;
}

void Pathfind::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::Pathfind msg;
    msg() = *data_;
    msg.Send( publisher );
}

void Pathfind::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    sword::SimToClient message;
    *message.mutable_message()->mutable_pathfind_update()= *data_;
    publisher.Send( message );
}

void Pathfind::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PathfindDestruction msg;
    msg().set_id( data_->id() );
    msg.Send( publisher );
}

void Pathfind::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
