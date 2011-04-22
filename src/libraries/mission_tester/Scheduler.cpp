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
#include "Filter_ABC.h"
#include "Schedulable_ABC.h"

namespace bpt = boost::posix_time;
using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Scheduler constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Scheduler::Scheduler( std::auto_ptr< Filter_ABC > filter, unsigned int delta )
    : filter_( filter )
    , last_  ( bpt::second_clock::local_time() )
    , delta_ ( delta )
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
void Scheduler::Schedule( boost::shared_ptr< Schedulable_ABC > schedulable )
{
    if( schedulable->Matches( *filter_ ) )
    {
        schedulables_.push_back( schedulable );
        next_ = schedulables_.begin();
    }
}

// -----------------------------------------------------------------------------
// Name: Scheduler::Step
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
void Scheduler::Step( Exercise& exercise )
{
    bpt::ptime current( bpt::microsec_clock::local_time() );
    if( last_ + bpt::milliseconds( delta_ ) < current )
    {
        if( !schedulables_.empty() )
        {
            if( next_ == schedulables_.end() )
                next_ = schedulables_.begin();
            if( (*next_)->StartMission( exercise ) )
                last_ = current;
            ++next_;
        }
    }
}
