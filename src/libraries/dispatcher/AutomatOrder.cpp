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
#include "protocol/ClientPublisher_ABC.h"
#include "Automat.h"

#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AutomatOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
AutomatOrder::AutomatOrder( Model_ABC& model, Automat& automat, const Common::MsgAutomatOrder& asn )
    : Order_ABC ( model, asn.mission(), asn.parametres() )
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
    asn().set_oid(automat_.GetId());
    asn().set_mission( missionID_ );
    Order_ABC::Send( *asn().mutable_parametres() );
    asn.Send( publisher );
    Delete( *asn().mutable_parametres() );
}

// -----------------------------------------------------------------------------
// Name: AutomatOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void AutomatOrder::SendNoMission( const Automat& automat, ClientPublisher_ABC& publisher )
{
    client::AutomatOrder asn;
    asn().set_oid          ( automat.GetId() );
    asn().set_mission      ( 0 );
    asn().mutable_parametres(); // $$$$ FHD 2009-10-28: should be removed if field is optional inprotocol
    asn.Send( publisher );
}
