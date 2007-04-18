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
Fire::Fire( Model& , const ASN1T_MsgStartPionFire& msg )
    : oid_tir         ( msg.oid_tir )
    , tireur          ( msg.tireur )
    , type            ( msg.type )
    , munitionPresent_( msg.m.munitionPresent != 0 )
    , munition        ( msg.munition )
    , typeCible_      ( msg.cible.t )
{
    if( typeCible_ == T_MsgStartPionFire_cible_position )
        positionCible_ = Position( *msg.cible.u.position );
    else
        oid_cible_ = msg.cible.u.pion;
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
void Fire::Update( const ASN1T_MsgStartPionFire& )
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
    AsnMsgInClientStartPionFire asn;
    asn().oid_tir = oid_tir;
    asn().tireur  = tireur;
    asn().type    = type;
    asn().m.munitionPresent = munitionPresent_ ? 1 : 0;
    asn().munition = munition;
    asn().cible.t = typeCible_;
    ASN1T_CoordUTM coord;
    if( typeCible_ == T_MsgStartPionFire_cible_position )
    {
        asn().cible.u.position = &coord;
        positionCible_.Send( coord );
    }
    else
        asn().cible.u.pion = oid_cible_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Fire::CommitDestruction
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Fire::CommitDestruction()
{
    AsnMsgInClientStopPionFire asn;
    asn().oid_tir = oid_tir;
    asn().degats_pions.n    = asn().degats_populations.n    = 0;
    asn().degats_pions.elem = 0;
    asn().degats_populations.elem = 0;
    Send( asn );
}
