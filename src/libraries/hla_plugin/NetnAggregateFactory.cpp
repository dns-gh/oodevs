// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnAggregateFactory.h"
#include "NetnAggregate.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnAggregateFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnAggregateFactory::NetnAggregateFactory( const AggregateFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregateFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnAggregateFactory::~NetnAggregateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnAggregateFactory::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< Aggregate_ABC > NetnAggregateFactory::Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
{
    std::auto_ptr< Aggregate_ABC > aggregate = factory_.Create( agent, name, identifier, force, type );
    return std::auto_ptr< Aggregate_ABC >( new NetnAggregate( aggregate, agent, name, identifier ) );
}
