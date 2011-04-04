// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "AgentFilter.h"
#include "Schedulable_ABC.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: AgentFilter constructor
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
AgentFilter::AgentFilter( const Criterion_ABC& criterion )
    : criterion_( criterion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFilter destructor
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
AgentFilter::~AgentFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFilter::Accepts
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool AgentFilter::Accepts( const Schedulable_ABC& schedulable ) const
{
    return schedulable.Matches( criterion_ );
}
