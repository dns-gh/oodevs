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
#include "ClientPublisher_ABC.h"

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
    , nSimState_           ( EnumSimulationState::stopped )
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
void SimulationModel::Update( const ASN1T_MsgControlInformation& msg )
{
    initialDate_          = std::string( (const char*)msg.initial_date_time.data, 15 );
    date_                 = initialDate_;
    nCurrentTick_         = msg.current_tick;
    nTickDuration_        = msg.tick_duration;
    nTimeFactor_          = msg.time_factor;
    nCheckpointFrequency_ = msg.checkpoint_frequency;
    nSimState_            = msg.status;
    bSendVisionCones_     = msg.send_vision_cones;
    bProfilingEnabled_    = msg.profiling_enabled;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Stop
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Stop( const ASN1T_MsgControlStopAck& msg )
{
    if( msg == EnumControlErrorCode::no_error )
        nSimState_ = EnumSimulationState::stopped;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Pause
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Pause( const ASN1T_MsgControlPauseAck& msg )
{
    if( msg == EnumControlErrorCode::no_error )
        nSimState_ = EnumSimulationState::paused;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Resume
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Resume( const ASN1T_MsgControlResumeAck& msg )
{
    if( msg == EnumControlErrorCode::no_error )
        nSimState_ = EnumSimulationState::running;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update( const ASN1T_MsgControlChangeTimeFactorAck& msg )
{
    if( msg.error_code == EnumControlErrorCode::no_error )
        nTimeFactor_ = msg.time_factor;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const ASN1T_MsgControlBeginTick& msg )
{
    nCurrentTick_ = msg.current_tick;
    date_ = std::string( (const char*)msg.date_time.data, 15 );
    if( initialDate_.empty() )
        initialDate_ = date_;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const ASN1T_MsgControlEndTick& msg )
{
    nCurrentTick_ = msg.current_tick;
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
//void SimulationModel::Update( const ASN1T_MsgControlCheckPointSetFrequencyAck& msg )
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
    asn().current_tick         = nCurrentTick_;
    asn().initial_date_time    = initialDate_.c_str();
    asn().date_time            = date_.c_str();
    asn().tick_duration        = nTickDuration_;
    asn().time_factor          = nTimeFactor_;
    asn().checkpoint_frequency = nCheckpointFrequency_;
    asn().status               = nSimState_;
    asn().send_vision_cones    = bSendVisionCones_;
    asn().profiling_enabled    = bProfilingEnabled_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::SendReplayInfo
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationModel::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, ASN1T_EnumSimulationState status, unsigned factor ) const
{
    replay::ControlReplayInformation asn;
    asn().current_tick      = nCurrentTick_;
    asn().initial_date_time = initialDate_.c_str();
    asn().date_time         = date_.c_str();
    asn().tick_duration     = nTickDuration_;
    asn().time_factor       = factor;
    asn().status            = status;
    asn().tick_count        = totalTicks;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::SendFirstTick
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void SimulationModel::SendFirstTick( ClientPublisher_ABC& publisher ) const
{
    client::ControlBeginTick asn;
    asn().current_tick = 0;
    asn().date_time    = date_.c_str();
    asn.Send( publisher );
}
