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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Fire constructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
Fire::Fire( Model& , const MsgsSimToClient::MsgStartUnitFire& msg )
    : SimpleEntity< > ( msg.fire_oid() )
    , type_           ( msg.type() )    
{
    oid_tir_.set_oid( msg.fire_oid() );
    tireur_.set_oid( msg.firer_oid() );
    munition_.set_oid( msg.ammunition() );

    if( msg.target().has_position() )
        positionCible_ = msg.target().position();
    else if( msg.target().has_unit() )
        oid_cible_ = msg.target().unit();
    else if( msg.target().has_population() )
        population_cible_ = msg.target().population();
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
    asn().set_fire_oid( oid_tir_.oid() );
    asn().set_firer_oid( tireur_.oid() );
    asn().set_type( type_ );    
    asn().set_ammunition( munition_.oid() );

    Common::MsgCoordLatLong coord;
    if( asn().target().has_position() )
    {
        *asn().mutable_target()->mutable_position() = coord;
        coord = positionCible_;
    }
    else
    {
        asn().mutable_target()->set_unit( oid_cible_ );
        asn().mutable_target()->set_population( population_cible_ );
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
    asn().set_fire_oid( oid_tir_.oid() );
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

