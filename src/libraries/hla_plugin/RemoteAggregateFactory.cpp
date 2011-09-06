// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAggregateFactory.h"
#include "RemoteAggregate.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAggregateFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregateFactory::RemoteAggregateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregateFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregateFactory::~RemoteAggregateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregateFactory::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< Aggregate_ABC > RemoteAggregateFactory::Create( const std::string& /*name*/, RemoteAgentListener_ABC& listener ) const
{
    return std::auto_ptr< Aggregate_ABC >( new RemoteAggregate( listener ) );
}
