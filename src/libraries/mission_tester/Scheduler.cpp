// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Filter_ABC.h"
#include "Schedulable_ABC.h"
#include "Scheduler.h"
#include "clients_kernel/Agent_ABC.h"
#include "protocol/protocol.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Scheduler constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Scheduler::Scheduler( const Filter_ABC& filter )
    : filter_( filter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Scheduler destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Scheduler::~Scheduler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Schedule
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Scheduler::Schedule( Schedulable_ABC& schedulable )
{
    if( filter_.Accepts( schedulable ) )
        shedulables_.push_back( &schedulable );
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Tick
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
void Scheduler::Step( unsigned int /*delta*/ )
{
    throw std::runtime_error( "to be implemented" );
}
