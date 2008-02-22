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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Lima::Update( const ASN1T_MsgLimaCreation& message )
{
    TacticalLine_ABC::Update( message.tactical_line );
    ApplyUpdate( message );
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
void Lima::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientLimaCreation asn;

    asn().oid = GetID();
    TacticalLine_ABC::Send( asn().tactical_line );
    asn.Send( publisher );

    Localisation::AsnDelete( asn().tactical_line.geometry );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendFullUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Lima::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Lima::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientLimaDestruction asn;
    asn() = GetID();
    asn.Send( publisher );
}
