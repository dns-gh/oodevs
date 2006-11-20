// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Lima.h"
#include "Model.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::Lima( const Model& model, const ASN1T_MsgLimaCreation& message )
    : TacticalLine_ABC( model, message.oid, message.tactical_line )
{
}
    
// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::~Lima()
{
}

// -----------------------------------------------------------------------------
// Name: Lima::Update
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Lima::Update( const ASN1T_MsgLimaUpdate& message )
{
    TacticalLine_ABC::Update( message.tactical_line );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendCreation
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Lima::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientLimaCreation asn;

    asn().oid = GetID();
    Send( asn().tactical_line );
    asn.Send( publisher );

    Localisation::AsnDelete( asn().tactical_line.geometrie );
}
