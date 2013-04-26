// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DetectionRangeEffect.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DetectionRangeEffect constructor
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
DetectionRangeEffect::DetectionRangeEffect( Model&, const sword::IndirectFirePerception& message )
    : SimpleEntity< >( message.fire_effects( 0 ).id() )
{
    for( int i = 0; i < message.perceivers_size(); ++i )
        perceivers_.push_back( message.perceivers( i ).id() );
    ammunition_ = message.ammunition().id();
    for( int i = 0; i < message.fire_effects_size(); ++i )
        fireEffects_.push_back( message.fire_effects( i ).id() );
}

// -----------------------------------------------------------------------------
// Name: DetectionRangeEffect destructor
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
DetectionRangeEffect::~DetectionRangeEffect()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionRangeEffect::SendFullUpdate
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void DetectionRangeEffect::SendFullUpdate( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionRangeEffect::SendCreation
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void DetectionRangeEffect::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::IndirectFirePerception msg;
    for( auto it = perceivers_.begin(); it != perceivers_.end(); ++it )
        msg().add_perceivers()->set_id( *it );
    msg().mutable_ammunition()->set_id( ammunition_ );
    for( auto it = fireEffects_.begin(); it != fireEffects_.end(); ++it )
        msg().add_fire_effects()->set_id( *it );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: DetectionRangeEffect::SendDestruction
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void DetectionRangeEffect::SendDestruction( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionRangeEffect::Accept
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void DetectionRangeEffect::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
