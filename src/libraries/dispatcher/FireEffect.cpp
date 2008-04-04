// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FireEffect.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FireEffect constructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
FireEffect::FireEffect( Model& , const ASN1T_MsgStartFireEffect& message )
    : id_          ( message.effect_oid )
    , localisation_( message.location )
    , type_        ( message.type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireEffect destructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
FireEffect::~FireEffect()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireEffect::SendFullUpdate
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void FireEffect::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireEffect::SendCreation
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void FireEffect::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientStartFireEffect asn;
    asn().effect_oid = id_;
    localisation_.Send( asn().location );
    asn().type = type_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: FireEffect::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void FireEffect::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientStopFireEffect asn;
    asn() = id_;
    asn.Send( publisher );
}
