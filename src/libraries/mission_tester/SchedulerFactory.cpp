// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "SchedulerFactory.h"
#include "FilterFactory.h"
#include "Scheduler.h"
#include "Filter_ABC.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: SchedulerFactory constructor
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
SchedulerFactory::SchedulerFactory( unsigned int delta, bool withFragOrders )
    : filterFactory_   ( new FilterFactory() )
    , delta_           ( delta )
    , withFragOrders_  ( withFragOrders )
    , recompletionFreq_( 10 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SchedulerFactory destructor
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
SchedulerFactory::~SchedulerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SchedulerFactory::CreateAgentScheduler
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
std::auto_ptr< Scheduler_ABC > SchedulerFactory::CreateAgentScheduler() const
{
    return std::auto_ptr< Scheduler_ABC >( new Scheduler( filterFactory_->Create( "agent" ), delta_, withFragOrders_, recompletionFreq_ ) );
}

// -----------------------------------------------------------------------------
// Name: SchedulerFactory::CreateAutomatScheduler
// Created: PHC 2011-05-12
// -----------------------------------------------------------------------------
std::auto_ptr< Scheduler_ABC > SchedulerFactory::CreateAutomatScheduler() const
{
    return std::auto_ptr< Scheduler_ABC >( new Scheduler( filterFactory_->Create( "automat" ), delta_, withFragOrders_, recompletionFreq_ ) );
}
