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
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"
#include "protocol/replaysenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationModel constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SimulationModel::SimulationModel()
    : nCurrentTick_        ( 0 )
    , nTickDuration_       ( 0 )
    , nTimeFactor_         ( 0 )
    , nCheckpointFrequency_( 0 )
    , nSimState_           ( Common::stopped )
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
void SimulationModel::Update( const MsgsSimToClient::MsgControlInformation& msg )
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
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Stop
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Stop( const MsgsSimToClient::MsgControlStopAck& msg )
{
    if( msg.error_code() == MsgsSimToClient::ControlAck_ErrorCode_no_error )
        nSimState_ = Common::stopped;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Pause
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Pause( const MsgsSimToClient::MsgControlPauseAck& msg )
{
    if( msg.error_code() == MsgsSimToClient::ControlAck_ErrorCode_no_error )
        nSimState_ = Common::paused;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Resume
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Resume( const MsgsSimToClient::MsgControlResumeAck& msg )
{
    if( msg.error_code() == MsgsSimToClient::ControlAck_ErrorCode_no_error )
        nSimState_ = Common::running;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update( const MsgsSimToClient::MsgControlChangeTimeFactorAck& msg )
{
    if( msg.error_code() == MsgsSimToClient::ControlAck_ErrorCode_no_error )
        nTimeFactor_ = msg.time_factor();
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const MsgsSimToClient::MsgControlBeginTick& msg )
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
void SimulationModel::Update( const MsgsSimToClient::MsgControlEndTick& msg )
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
//void SimulationModel::Update( const MsgControlCheckPointSetFrequencyAck& msg )
//{
//    nCheckpointFrequency_ = msg;
//}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Send
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Send( ClientPublisher_ABC& publisher ) const
{
    client::ControlInformation asn;
    asn().set_current_tick         ( nCurrentTick_);
    asn().mutable_initial_date_time()->set_data( initialDate_.c_str() );
    asn().mutable_date_time()->set_data( date_.c_str() );
    asn().set_tick_duration        ( nTickDuration_);
    asn().set_time_factor          ( nTimeFactor_);
    asn().set_checkpoint_frequency ( nCheckpointFrequency_);
    asn().set_status               ( nSimState_);
    asn().set_send_vision_cones    (bSendVisionCones_);
    asn().set_profiling_enabled    ( bProfilingEnabled_);
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::SendReplayInfo
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationModel::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, Common::EnumSimulationState status, unsigned factor ) const
{
    replay::ControlReplayInformation asn;
    asn().set_current_tick      ( nCurrentTick_ );
    asn().mutable_initial_date_time()->set_data( initialDate_.c_str() );
    asn().mutable_date_time()->set_data( date_.c_str() );
    asn().set_tick_duration     ( nTickDuration_ );
    asn().set_time_factor       ( factor );
    asn().set_status            ( status );
    asn().set_tick_count        ( totalTicks );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::SendFirstTick
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationModel::SendFirstTick( ClientPublisher_ABC& publisher ) const
{
    client::ControlBeginTick asn;
    asn().set_current_tick ( 0 );
    asn().mutable_date_time()->set_data( date_.c_str() );
    asn.Send( publisher );
}
