// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationModel.h"
#include "Config.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/ReplaySenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationModel constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SimulationModel::SimulationModel( const Config& config )
    : nCurrentTick_        ( 0 )
    , nTickDuration_       ( config.GetTickDuration() )
    , nTimeFactor_         ( 0 )
    , nCheckpointFrequency_( 0 )
    , nSimState_           ( sword::stopped )
    , bSendVisionCones_    ( false )
    , bProfilingEnabled_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationModel destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SimulationModel::~SimulationModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Reset
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void SimulationModel::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update( const sword::ControlInformation& msg )
{
    initialDate_          = msg.initial_date_time().data();
    date_                 = initialDate_;
    nCurrentTick_         = msg.current_tick();
    nTickDuration_        = msg.tick_duration();
    nTimeFactor_          = msg.time_factor();
    nCheckpointFrequency_ = msg.checkpoint_frequency();
    nSimState_            = msg.status();
    bSendVisionCones_     = msg.send_vision_cones() != 0;
    bProfilingEnabled_    = msg.profiling_enabled() != 0;
    if( msg.has_checkpoint_real_time() )
        localTime_ = msg.checkpoint_real_time().data();
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Stop
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Stop( const sword::ControlStopAck& msg )
{
    if( msg.error_code() == sword::ControlAck::no_error )
        nSimState_ = sword::stopped;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Pause
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Pause( const sword::ControlPauseAck& msg )
{
    if( msg.error_code() == sword::ControlAck::no_error )
        nSimState_ = sword::paused;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Resume
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Resume( const sword::ControlResumeAck& msg )
{
    if( msg.error_code() == sword::ControlAck::no_error )
        nSimState_ = sword::running;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update( const sword::ControlChangeTimeFactorAck& msg )
{
    if( msg.error_code() == sword::ControlAck::no_error )
        nTimeFactor_ = msg.time_factor();
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const sword::ControlBeginTick& msg )
{
    nCurrentTick_ = msg.current_tick();
    date_ = msg.date_time().data();
    if( initialDate_.empty() )
        initialDate_ = date_;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const sword::ControlEndTick& msg )
{
    nCurrentTick_ = msg.current_tick();
    //$$$$
    /*
    tick-duration       INTEGER,    -- En millisecondes
    nb-pathfinds-longs  INTEGER,
    nb-pathfinds-courts INTEGER
    */
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
//void SimulationModel::Update( const ControlCheckPointSetFrequencyAck& msg )
//{
//    nCheckpointFrequency_ = msg;
//}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Send
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Send( ClientPublisher_ABC& publisher ) const
{
    client::ControlInformation msg;
    msg().set_current_tick         ( nCurrentTick_);
    msg().mutable_initial_date_time()->set_data( initialDate_.c_str() );
    msg().mutable_date_time()->set_data( date_.c_str() );
    msg().set_tick_duration        ( nTickDuration_ );
    msg().set_time_factor          ( nTimeFactor_);
    msg().set_checkpoint_frequency ( nCheckpointFrequency_ );
    msg().set_status               ( nSimState_);
    msg().set_send_vision_cones    ( bSendVisionCones_ );
    msg().set_profiling_enabled    ( bProfilingEnabled_ );
    if( localTime_ != "" )
        msg().mutable_checkpoint_real_time()->set_data( localTime_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::SendReplayInfo
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationModel::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, sword::EnumSimulationState status, unsigned factor ) const
{
    replay::ControlReplayInformation msg;
    msg().set_current_tick      ( nCurrentTick_ );
    msg().mutable_initial_date_time()->set_data( initialDate_.c_str() );
    msg().mutable_date_time()->set_data( date_.c_str() );
    msg().set_tick_duration     ( nTickDuration_ );
    msg().set_time_factor       ( factor );
    msg().set_status            ( status );
    msg().set_tick_count        ( totalTicks );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::SendFirstTick
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationModel::SendFirstTick( ClientPublisher_ABC& publisher ) const
{
    client::ControlBeginTick msg;
    msg().set_current_tick ( 0 );
    msg().mutable_date_time()->set_data( date_.c_str() );
    msg.Send( publisher );
}
