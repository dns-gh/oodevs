// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "AggregatedState.h"
#include "Agent.h"

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: AggregatedState constructor
// Created: SBO 2011-04-11
// -----------------------------------------------------------------------------
AggregatedState::AggregatedState( const Agent& agent )
    : agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregatedState destructor
// Created: SBO 2011-04-11
// -----------------------------------------------------------------------------
AggregatedState::~AggregatedState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregatedState::Update
// Created: SBO 2011-04-11
// -----------------------------------------------------------------------------
void AggregatedState::Update( const DtEntityIdentifier& identifier, const DtDamageState& state )
{
    DtDamageState& last = states_[ identifier ];
    if( last != state )
    {
        last = state;
        agent_.NotifyStateChanged();
    }
}
