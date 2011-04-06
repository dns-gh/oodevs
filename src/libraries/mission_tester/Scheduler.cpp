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
// Name: Scheduler constructor
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
Scheduler::Scheduler( Filter_ABC& filter )
    : filter_( &filter )
    , last_  ( bpt::second_clock::local_time() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Scheduler destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Scheduler::~Scheduler()
{
    criteria_.release();
    filter_.release();
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Schedule
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Scheduler::Schedule( Schedulable_ABC& schedulable )
{
    if( filter_->Accepts( schedulable ) )
    { 
        schedulables_.push_back( &schedulable );
        next_ = schedulables_.begin();
    }
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Tick
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
void Scheduler::Step( unsigned int delta )
{
    bpt::ptime current( bpt::microsec_clock::local_time() );
    if( last_ + bpt::milliseconds( delta ) < current )
    {
        last_ = current;
        if( !schedulables_.empty() )
        {
            if( next_ == schedulables_.end() )
                next_ = schedulables_.begin();
            std::cout << "Launching a new mission for " + (*next_)->SchedulableName() << std::endl;
            ++next_;
        }
    }
}
