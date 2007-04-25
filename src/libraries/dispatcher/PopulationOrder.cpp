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
void PopulationOrder::Send( Publisher_ABC& publisher )
{
    AsnMsgSimToClientPopulationOrder asn;
    asn().oid_unite_executante = population_.GetID();
    asn().mission              = missionID_;
    
    Order_ABC::Send( asn().parametres );

    asn.Send( publisher );

    Order_ABC::AsnDelete( asn().parametres );    
}
