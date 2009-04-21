// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AutomatOrder.h"
#include "ClientPublisher_ABC.h"
#include "Automat.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AutomatOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AutomatOrder::AutomatOrder( Model& model, Automat& automat, const ASN1T_MsgAutomatOrder& asn )
    : Order_ABC ( model, asn.mission, asn.parametres )
    , automat_  ( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AutomatOrder::~AutomatOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void AutomatOrder::Send( ClientPublisher_ABC& publisher )
{
    client::AutomatOrder asn;
    asn().oid       = automat_.GetId();
    asn().mission   = missionID_;   
    Order_ABC::Send( asn().parametres );
    asn.Send( publisher );
    AsnDelete( asn().parametres );    
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void AutomatOrder::SendNoMission( const Automat& automat, ClientPublisher_ABC& publisher )
{
    client::AutomatOrder asn;
    asn().oid          = automat.GetId();
    asn().mission      = 0;
    asn().parametres.n = 0;
    asn.Send( publisher );
}
