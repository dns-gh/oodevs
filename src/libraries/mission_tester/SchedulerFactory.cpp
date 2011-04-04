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
#include "AgentFilter.h"
#include "Scheduler.h"

using namespace mission_tester;



// -----------------------------------------------------------------------------
// Name: SchedulerFactory constructor
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
SchedulerFactory::SchedulerFactory()
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
Scheduler_ABC* SchedulerFactory::CreateAgentScheduler()
{
    std::auto_ptr< Filter_ABC > filter ( new AgentFilter( agentCriterion_ ) );
    boost::shared_ptr< Scheduler_ABC > scheduler( new Scheduler( *filter ) );
    schedulers_.push_back( scheduler );
    return scheduler.get();
}
