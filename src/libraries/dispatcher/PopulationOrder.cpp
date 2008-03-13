// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationOrder.h"
#include "Network_Def.h"
#include "Population.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
PopulationOrder::PopulationOrder( Model& model, Population& population, const ASN1T_MsgPopulationOrder& asn )
    : Order_ABC  ( model, asn.mission, asn.parametres )
    , population_( population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
PopulationOrder::~PopulationOrder()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void PopulationOrder::Send( ClientPublisher_ABC& publisher )
{
    AsnMsgSimToClientPopulationOrder asn;
    asn().oid       = population_.GetID();
    asn().mission   = missionID_;
    
    Order_ABC::Send( asn().parametres );

    asn.Send( publisher );

    AsnDelete( asn().parametres );    
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void PopulationOrder::SendNoMission( const Population& population, ClientPublisher_ABC& publisher )
{
    AsnMsgSimToClientPopulationOrder asn;

    asn().oid          = population.GetID();
    asn().mission      = 0;
    asn().parametres.n = 0;
    asn.Send( publisher );
}
