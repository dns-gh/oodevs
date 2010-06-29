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
#include "protocol/ClientPublisher_ABC.h"
#include "Population.h"

#include "protocol/clientsenders.h"

using namespace dispatcher;
//using namespace Common;

// -----------------------------------------------------------------------------
// Name: PopulationOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
PopulationOrder::PopulationOrder( Model_ABC& model, Population& population, const Common::MsgPopulationOrder& asn )
    : Order_ABC  ( model, asn.mission(), asn.parametres() )
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
    client::PopulationOrder asn;
    asn().set_oid( population_.GetId() );
    asn().set_mission( missionID_ );

    Order_ABC::Send( *asn().mutable_parametres() );

    asn.Send( publisher );

    Delete( *asn().mutable_parametres() );
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void PopulationOrder::SendNoMission( const Population& population, ClientPublisher_ABC& publisher )
{
    client::PopulationOrder asn;

    asn().set_oid( population.GetId() );
    asn().set_mission( 0 );
    asn.Send( publisher );
}
