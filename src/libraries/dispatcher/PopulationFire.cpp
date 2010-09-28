// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationFire.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFire constructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
PopulationFire::PopulationFire( Model& , const MsgsSimToClient::MsgStartCrowdFire& msg )
    : SimpleEntity < >( msg.fire().id() )
    , msg_( msg )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire destructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
PopulationFire::~PopulationFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::SendFullUpdate
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void PopulationFire::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::SendCreation
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void PopulationFire::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::StartCrowdFire message;
    message() = msg_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFire::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::StopCrowdFire message;
    message().mutable_fire()->set_id( msg_.fire().id() );
    message().mutable_units_damages();
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationFire::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
