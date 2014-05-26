// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Action.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

Action::Action( const Model& /*model*/, const sword::Action& msg )
    : SimpleEntity<>( msg.id() )
    , action_( new sword::Action( msg ) )
{
    // NOTHING
}

Action::~Action()
{
    // NOTHING
}

void Action::DoUpdate( const sword::Action& msg )
{
    *action_ = msg;
}

void Action::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::Action msg;
    msg() = *action_;
    msg.Send( publisher );
}

void Action::SendFullUpdate( ClientPublisher_ABC& /*publisher */) const
{
    // NOTHING
}

void Action::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ActionDestruction msg;
    msg().set_id( action_->id() );
    msg.Send( publisher );
}

void Action::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}