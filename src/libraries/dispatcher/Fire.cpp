// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Fire.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Fire constructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
Fire::Fire( Model& , const MsgsSimToClient::MsgStartUnitFire& msg )
    : SimpleEntity< >  ( msg.id().id() )
    , type_            ( msg.type() )
    , oid_cible_       ( 0 )
    , population_cible_( 0 )
{
    oid_tir_.set_id( msg.id().id() );
    tireur_.set_id( msg.firing_unit().id() );
    munition_.set_id( msg.ammunition().id() );

    if( msg.target().has_position() )
        positionCible_ = msg.target().position();
    else if( msg.target().has_unit() )
        oid_cible_ = msg.target().unit().id();
    else if( msg.target().has_population() )
        population_cible_ = msg.target().population().id();
}

// -----------------------------------------------------------------------------
// Name: Fire destructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
Fire::~Fire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Fire::SendFullUpdate
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Fire::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Fire::SendCreation
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Fire::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::StartUnitFire asn;
    asn().mutable_id()->set_id( oid_tir_.id() );
    asn().mutable_firing_unit()->set_id( tireur_.id() );
    asn().set_type( type_ );
    asn().mutable_ammunition()->set_id( munition_.id() );

    Common::MsgCoordLatLong coord;
    if( asn().target().has_position() )
    {
        *asn().mutable_target()->mutable_position() = coord;
        coord = positionCible_;
    }
    else
    {
        if( oid_cible_ )
            asn().mutable_target()->mutable_unit()->set_id( oid_cible_ );
        if( population_cible_ )
            asn().mutable_target()->mutable_population()->set_id( population_cible_ );
    }
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Fire::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Fire::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::StopUnitFire asn;
    asn().mutable_id()->set_id( oid_tir_.id() );
    asn().mutable_units_damages(); //->set_n(0);
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Fire::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Fire::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

