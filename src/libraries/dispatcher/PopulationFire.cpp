// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationFire.h"
#include "network_def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFire constructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
PopulationFire::PopulationFire( Model& , const ASN1T_MsgStartPopulationFire& msg )
    : msg_( msg )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire destructor
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
PopulationFire::~PopulationFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::Update
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void PopulationFire::Update( const ASN1T_MsgStartPopulationFire& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::SendFullUpdate
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void PopulationFire::SendFullUpdate( Publisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::SendCreation
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void PopulationFire::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientStartPopulationFire asn;
    asn() = msg_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFire::CommitDestruction
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void PopulationFire::CommitDestruction()
{
    AsnMsgInClientStopPopulationFire asn;
    asn().oid_tir = msg_.oid_tir;
    asn().degats_pions.n    = 0;
    asn().degats_pions.elem = 0;
    Send( asn );
}
