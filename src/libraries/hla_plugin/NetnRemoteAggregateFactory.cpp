// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteAggregateFactory.h"
#include "NetnRemoteAggregate.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregateFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAggregateFactory::NetnRemoteAggregateFactory( const RemoteAggregateFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregateFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteAggregateFactory::~NetnRemoteAggregateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteAggregateFactory::Create
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< Aggregate_ABC > NetnRemoteAggregateFactory::Create( const std::string& name ) const
{
    std::auto_ptr< Aggregate_ABC > remote = factory_.Create( name );
    return std::auto_ptr< Aggregate_ABC >( new NetnRemoteAggregate( remote ) );
}
