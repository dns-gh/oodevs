// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionsScheduler.h"
#include "Simulation.h"
#include "Action_ABC.h"
#include "ActionsModel.h"
#include "ActionTiming.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionsScheduler constructor
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
ActionsScheduler::ActionsScheduler( Controllers& controllers, const Simulation& simulation, const ActionsModel& actions, Publisher_ABC& publisher )
    : controllers_( controllers )
    , simulation_( simulation )
    , actions_( actions )
    , publisher_( publisher )
    , startTime_( 0 )
    , running_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler destructor
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
ActionsScheduler::~ActionsScheduler()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::Start
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::Start()
{
    startTime_ = simulation_.GetCurrentTick();
    running_ = true;
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::Stop
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::Stop()
{
    running_ = false;
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::NotifyUpdated
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::NotifyUpdated( const Simulation::sStartTick& )
{
    if( !running_ || simulation_.GetCurrentTick() < startTime_ )
        return;
    unsigned long currentTime = simulation_.GetCurrentTick() - startTime_;

    Iterator< const Action_ABC& > it( actions_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const Action_ABC& action = it.NextElement();
        if( const ActionTiming* timing = action.Retrieve< ActionTiming >() )
            if( timing->IsEnabled() && timing->GetTime() == currentTime )
                action.Publish( publisher_ );
    }
}
