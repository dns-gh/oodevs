// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SimulationController.h"
#include "Services.h"
#include "Simulation.h"
#include "clients_kernel/Tools.h"
#include "protocol/Dispatcher.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/ReplaySenders.h"
#include "protocol/SimulationSenders.h"
#include "actions/Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: SimulationController constructor
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
SimulationController::SimulationController( const Simulation& simulation, kernel::Controllers& controllers, Publisher_ABC& publisher )
    : simulation_( simulation )
    , controllers_( controllers )
    , publisher_( publisher )
    , hasReplay_( false )
    , hasSimulation_( false )
    , replayRequested_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationController destructor
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
SimulationController::~SimulationController()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::IsPaused
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
bool SimulationController::IsPaused() const
{
    return simulation_.IsPaused();
}

// -----------------------------------------------------------------------------
// Name: SimulationController::GetFirstTick
// Created: JSR 2013-10-30
// -----------------------------------------------------------------------------
unsigned int SimulationController::GetFirstTick() const
{
    return simulation_.GetFirstTick();
}

// -----------------------------------------------------------------------------
// Name: SimulationController::GetTickCount
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
unsigned int SimulationController::GetTickCount() const
{
    return simulation_.GetTickCount();
}

// -----------------------------------------------------------------------------
// Name: SimulationController::GetTickDuration
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
unsigned int SimulationController::GetTickDuration() const
{
    return simulation_.GetTickDuration();
}

// -----------------------------------------------------------------------------
// Name: SimulationController::Pause
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
void SimulationController::Pause()
{
    if( hasReplay_ )
    {
        replayRequested_ = false;
        replay::ControlPause message;
        message.Send( publisher_ );
    }
    if( hasSimulation_ )
    {
        simulation::ControlPause message;
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::Resume
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
void SimulationController::Resume()
{
    if( hasReplay_ )
    {
        replayRequested_ = true;
        replay::ControlResume message;
        message.Send( publisher_ );
    }
    if( hasSimulation_ )
    {
        simulation::ControlResume  message;
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::Step
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
void SimulationController::Step()
{
    if( hasReplay_ )
    {
        if( !IsPaused() )
            Pause();
        else
        {
            replay::ControlResume message;
            message().set_tick( 1 );
            message.Send( publisher_ );
        }
    }
    if( hasSimulation_ )
    {
        simulation::ControlResume  message;
        message().set_tick( 1 );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::ChangeTimeFactor
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
void SimulationController::ChangeTimeFactor( int factor )
{
    if( hasReplay_ )
    {
        replay::ControlChangeTimeFactor message;
        message().set_time_factor( factor );
        message.Send( publisher_ );
    }
    if( hasSimulation_ )
    {
        simulation::ControlChangeTimeFactor message;
        message().set_time_factor( factor );
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::SkipToDate
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
void SimulationController::SkipToDate( const QDateTime& dateTime )
{
    replay::ControlSkipToDate skip;
    skip().mutable_date_time()->set_data( tools::QDateTimeToGDHString( dateTime ) );
    skip.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::SkipToTick
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
void SimulationController::SkipToTick( unsigned int tick )
{
    replay::ControlSkipToTick skip;
    skip().set_tick( tick );
    skip.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::RequestCheckpoint
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
void SimulationController::RequestCheckpoint( const std::string& name )
{
    simulation::ControlCheckPointSaveNow message;
    if( !name.empty() )
        message().set_name ( name.c_str() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::ForceReplayDataRequest
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
void SimulationController::ForceReplayDataRequest()
{
    replay::ForceRefreshDataRequest reload;
    reload.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::NotifyUpdated
// Created: JSR 2013-03-26
// -----------------------------------------------------------------------------
void SimulationController::NotifyUpdated( const Services& services )
{
    hasReplay_ = services.HasService( sword::service_replay );
    hasSimulation_ = services.HasService( sword::service_simulation );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::NotifyUpdated
// Created: JSR 2013-03-27
// -----------------------------------------------------------------------------
void SimulationController::NotifyUpdated( const Simulation::sEndTick& /*endTick*/ )
{
    if( replayRequested_ && hasReplay_ )
    {
        replay::ControlResume message;
        message.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::NotifyUpdated
// Created: JSR 2013-03-28
// -----------------------------------------------------------------------------
void SimulationController::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.IsPaused() )
        replayRequested_ = false;
}

// -----------------------------------------------------------------------------
// Name: SimulationController::SendEnableVisionCones
// Created: LGY 2014-01-08
// -----------------------------------------------------------------------------
void SimulationController::SendEnableVisionCones( bool value ) const
{
    if( hasSimulation_ )
    {
        simulation::ControlEnableVisionCones msg;
        msg().set_vision_cones( value );
        msg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::PublishAction
// Created: LGY 2014-01-08
// -----------------------------------------------------------------------------
void SimulationController::PublishAction( const actions::Action_ABC& action ) const
{
    if( hasSimulation_ )
        action.Publish( publisher_, 0 );
}

namespace
{
    template< typename T >
    void SendLogisticRequests( const std::set< unsigned long >& entities, Publisher_ABC& publisher,
                               unsigned int currentTick )
    {
        T msg;
        auto request = msg.mutable_message()->mutable_list_logistic_requests();
        request->set_current_tick( currentTick );
        for( auto it = entities.begin(); it != entities.end(); ++it )
            request->add_entities()->set_id( *it );
        publisher.Send( msg );
    }
    template< typename T >
    void SendHistoryRequests( const std::set< unsigned int >& requests, Publisher_ABC& publisher )
    {
        T msg;
        auto request = msg.mutable_message()->mutable_logistic_history_request();
        for( auto it = requests.begin(); it != requests.end(); ++it )
            request->add_requests()->set_id( *it );
        publisher.Send( msg );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationController::SendLogisticRequests
// Created: LGY 2014-01-07
// -----------------------------------------------------------------------------
void SimulationController::SendLogisticRequests( const std::set< unsigned long >& entities ) const
{
    unsigned int currentTick = simulation_.GetCurrentTick();
    if( hasReplay_ )
        ::SendLogisticRequests< sword::ClientToReplay >( entities, publisher_, currentTick );
    else if( hasSimulation_ )
        ::SendLogisticRequests< sword::ClientToSim >( entities, publisher_, currentTick );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::SendHistoryRequests
// Created: LGY 2014-01-07
// -----------------------------------------------------------------------------
void SimulationController::SendHistoryRequests( const std::set< unsigned int >& requests ) const
{
    if( hasReplay_ )
        ::SendHistoryRequests< sword::ClientToReplay >( requests, publisher_ );
    else if( hasSimulation_ )
        ::SendHistoryRequests< sword::ClientToSim >( requests, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::RegisterSimHandler
// Created: LGY 2014-01-07
// -----------------------------------------------------------------------------
void SimulationController::RegisterSimHandler( Publisher_ABC::T_SimHandler handler )
{
    publisher_.Register( handler );
}

// -----------------------------------------------------------------------------
// Name: SimulationController::RegisterReplayHandler
// Created: LGY 2014-01-07
// -----------------------------------------------------------------------------
void SimulationController::RegisterReplayHandler( Publisher_ABC::T_ReplayHandler handler )
{
    publisher_.Register( handler );
}

QDateTime SimulationController::GetTime( uint32_t tick ) const
{
    return simulation_.GetTime( tick );
}