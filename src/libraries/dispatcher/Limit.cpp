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
#include "ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Limit::Limit( const Model& model, const ASN1T_MsgLimitCreation& message )
    : TacticalLine_ABC( model, message.oid, message.tactical_line )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Limit::~Limit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Limit::Update( const ASN1T_MsgLimitCreation& message )
{
    FlagUpdate();
    TacticalLine_ABC::Update( message.tactical_line );
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
    TacticalLine_ABC::Send( asn().tactical_line );
    asn.Send( publisher );

    Localisation::AsnDelete( asn().tactical_line.geometrie );
}

// -----------------------------------------------------------------------------
// Name: Limit::SendFullUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Limit::SendFullUpdate( Publisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit::CommitDestruction
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Limit::CommitDestruction()
{
    AsnMsgInClientLimitDestruction asn;
    asn() = GetID();
    Entity_ABC::Send( asn );
}

// -----------------------------------------------------------------------------
// Name: Limit::Accept
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Limit::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
}
