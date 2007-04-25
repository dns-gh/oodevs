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
    : id_          ( message.oid_effet )
    , localisation_( message.localisation )
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
// Name: FireEffect::Update
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void FireEffect::Update( const ASN1T_MsgStartFireEffect& )
{
    FlagUpdate();
}
    
// -----------------------------------------------------------------------------
// Name: FireEffect::SendFullUpdate
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void FireEffect::SendFullUpdate( Publisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireEffect::SendCreation
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void FireEffect::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientStartFireEffect asn;
    asn().oid_effet = id_;
    localisation_.Send( asn().localisation );
    asn().type = type_;
    asn.Send( publisher );
    localisation_.AsnDelete( asn().localisation );
}

// -----------------------------------------------------------------------------
// Name: FireEffect::CommitDestruction
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void FireEffect::CommitDestruction()
{
    AsnMsgSimToClientStopFireEffect asn;
    asn() = id_;
    Send( asn );
}
