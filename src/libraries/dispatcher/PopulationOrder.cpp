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

// -----------------------------------------------------------------------------
// Name: PopulationOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
PopulationOrder::PopulationOrder( Model_ABC& model, Population& population, const Common::MsgPopulationOrder& asn )
    : Order_ABC  ( model, asn.type().id(), asn.parameters() )
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
    asn().mutable_tasker()->set_id( population_.GetId() );
    asn().mutable_type()->set_id( missionID_ );
    Order_ABC::Send( *asn().mutable_parameters() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationOrder::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void PopulationOrder::SendNoMission( const Population& population, ClientPublisher_ABC& publisher )
{
    client::PopulationOrder asn;
    asn().mutable_tasker()->set_id( population.GetId() );
    asn().mutable_type()->set_id( 0 );
    asn.Send( publisher );
}
