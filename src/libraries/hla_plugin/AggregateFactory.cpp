// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AggregateFactory.h"
#include "AggregateEntity.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AggregateFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
AggregateFactory::AggregateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
AggregateFactory::~AggregateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateFactory::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< Aggregate_ABC > AggregateFactory::Create( Agent_ABC& agent, const std::string& name, short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
{
    return std::auto_ptr< Aggregate_ABC >( new AggregateEntity( agent, identifier, name, force, type ) );
}
