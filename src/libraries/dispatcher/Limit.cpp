// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Limit.h"
#include "Model.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Limit::Limit( const Model& model, const ASN1T_MsgLimitCreation& message )
    : TacticalLine_ABC( model, message.oid, message.tactical_line )
{
}
    
// -----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Limit::~Limit()
{
}

// -----------------------------------------------------------------------------
// Name: Limit::Update
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Limit::Update( const ASN1T_MsgLimitUpdate& message )
{
    TacticalLine_ABC::Update( message.tactical_line );
}

// -----------------------------------------------------------------------------
// Name: Limit::SendCreation
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Limit::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLimitCreation asn;

    asn().oid = GetID();
    Send( asn().tactical_line );
    asn.Send( publisher );

    Localisation::AsnDelete( asn().tactical_line.geometrie );
}
