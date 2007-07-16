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
ActionsScheduler::ActionsScheduler( QObject* parent, Controllers& controllers, const Simulation& simulation, const ActionsModel& actions, Publisher_ABC& publisher )
    : QObject( parent )
    , controllers_( controllers )
    , simulation_( simulation )
    , actions_( actions )
    , publisher_( publisher )
    , currentTime_( 0 )
    , lastTick_( 0 )
    , running_( false )
    , paused_( false )
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
    if( running_ )
        paused_ = !paused_;
    else
    {
        lastTick_ = simulation_.GetCurrentTick();
        running_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::Stop
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::Stop()
{
    paused_ = false;
    running_ = false;
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::IsRunning
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
bool ActionsScheduler::IsRunning() const
{
    return running_ && !paused_;
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::NotifyUpdated
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::NotifyUpdated( const Simulation::sStartTick& )
{
    int interval = simulation_.GetCurrentTick() - lastTick_; // $$$$ SBO 2007-07-16: should provide replayer time-shifting compatibility
    lastTick_ = simulation_.GetCurrentTick();
    if( !running_ || paused_ || currentTime_ + interval < 0 )
        return;
    currentTime_ += interval;
    Iterator< const Action_ABC& > it( actions_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const Action_ABC& action = it.NextElement();
        if( const ActionTiming* timing = action.Retrieve< ActionTiming >() )
            if( timing->IsEnabled() && timing->GetTime() == currentTime_ )
                action.Publish( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::GetCurrentTime
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
unsigned long ActionsScheduler::GetCurrentTime() const
{
    return currentTime_;
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::Shift
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void ActionsScheduler::Shift( long shift )
{
    currentTime_ = std::max< long >( 0, long( currentTime_ ) + shift );
}
