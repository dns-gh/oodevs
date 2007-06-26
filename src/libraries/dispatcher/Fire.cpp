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
#include "network_def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Fire constructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
Fire::Fire( Model& , const ASN1T_MsgStartUnitFire& msg )
    : oid_tir         ( msg.fire_oid )
    , tireur          ( msg.firer_oid )
    , type            ( msg.type )
    , munitionPresent_( msg.m.ammunitionPresent != 0 )
    , munition        ( msg.ammunition )
    , typeCible_      ( msg.target.t )
{
    if( typeCible_ == T_MsgStartUnitFire_target_position )
        positionCible_ = Position( *msg.target.u.position );
    else
        oid_cible_ = msg.target.u.unit;
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
// Name: Fire::Update
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Fire::Update( const ASN1T_MsgStartUnitFire& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: Fire::SendFullUpdate
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Fire::SendFullUpdate( Publisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Fire::SendCreation
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Fire::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientStartUnitFire asn;
    asn().fire_oid  = oid_tir;
    asn().firer_oid = tireur;
    asn().type      = type;
    asn().m.ammunitionPresent = munitionPresent_ ? 1 : 0;
    asn().ammunition = munition;
    asn().target.t = typeCible_;
    ASN1T_CoordUTM coord;
    if( typeCible_ == T_MsgStartUnitFire_target_position )
    {
        asn().target.u.position = &coord;
        positionCible_.Send( coord );
    }
    else
        asn().target.u.unit = oid_cible_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Fire::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Fire::SendDestruction( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientStopUnitFire asn;
    asn().fire_oid = oid_tir;
    asn().units_damages.n    = asn().populations_damages.n    = 0;
    asn().units_damages.elem = 0;
    asn().populations_damages.elem = 0;
    asn.Send( publisher );
}
