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
#include "tools/App6Symbol.h"

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
void Limit::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientLimitCreation asn;

    asn().oid = GetID();
    TacticalLine_ABC::Send( asn().tactical_line );
    asn.Send( publisher );

    Localisation::AsnDelete( asn().tactical_line.geometry );
}

// -----------------------------------------------------------------------------
// Name: Limit::SendFullUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Limit::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Limit::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientLimitDestruction asn;
    asn() = GetID();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Limit::BuildSymbol
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
std::string Limit::BuildSymbol( bool up /*= true*/ ) const
{
    std::string symbol;
    tools::app6::SetFunction( symbol, "**B***" );
    if( up )
        return tools::app6::MergeSymbol( symbol, TacticalLine_ABC::BuildSymbol() );
    return symbol;
}
