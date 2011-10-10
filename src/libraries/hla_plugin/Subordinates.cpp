// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Subordinates.h"
#include "CallsignResolver_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include "tools/Resolver.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Agent_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Subordinates constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
Subordinates::Subordinates( const CallsignResolver_ABC& callsignResolver, const tools::Resolver_ABC< dispatcher::Automat_ABC, unsigned long >& automatResolver )
    : callsignResolver_( callsignResolver )
    , automatResolver_ ( automatResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Subordinates destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
Subordinates::~Subordinates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Subordinates::Apply
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void Subordinates::Apply( unsigned long automatIdentifier, TransportedUnitsVisitor_ABC& visitor ) const
{
    const tools::Resolver< dispatcher::Agent_ABC >& agentsResolver = automatResolver_.Get( automatIdentifier ).GetAgents();
    for( tools::Iterator< const dispatcher::Agent_ABC& > it = agentsResolver.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::Agent_ABC& agent = it.NextElement();
        visitor.Notify( callsignResolver_.ResolveCallsign( agent.GetId() ), callsignResolver_.ResolveUniqueId( agent.GetId() ) );
    }
}
