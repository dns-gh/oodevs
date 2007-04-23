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
#include "Network_Def.h"
#include "Automat.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AutomatOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AutomatOrder::AutomatOrder( Model& model, Automat& automat, const ASN1T_MsgAutomateOrder& asn )
    : Order_ABC ( model, asn.mission, asn.parametres, asn.order_context )
    , automat_  ( automat )
    , formation_( asn.formation )
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AutomatOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void AutomatOrder::Send( Publisher_ABC& publisher )
{
    AsnMsgInClientAutomateOrder asn;
    asn().oid_unite_executante = automat_.GetID();
    asn().mission              = missionID_;
    asn().formation            = formation_;       
    Order_ABC::Send( asn().order_context );
    Order_ABC::Send( asn().parametres );

    asn.Send( publisher );

    Order_ABC::AsnDelete( asn().order_context );
    Order_ABC::AsnDelete( asn().parametres );    
}
