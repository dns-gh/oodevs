// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Scheduler.h"
#include "Criteria.h"
#include "Filter.h"
#include "Schedulable_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "protocol/protocol.h"

namespace bpt = boost::posix_time;
using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Scheduler constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Scheduler::Scheduler( const std::string& criteria )
    : criteria_( new Criteria( criteria ) )
    , filter_  ( new Filter( *criteria_ ) )
    , last_    ( bpt::second_clock::local_time() )
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
    if( filter_->Accepts( schedulable ) )
        shedulables_.push_back( &schedulable );
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Tick
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
void Scheduler::Step( unsigned int delta )
{
    bpt::ptime current( bpt::second_clock::local_time() );
    if( last_ + bpt::milliseconds( delta ) < current )
    {
        last_ = current;
        std::cout << "tick!" << std::endl;
    }
}
