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
#include "Network_Def.h"
#include "DIN/DIN_Input.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"

using namespace dispatcher;
using namespace DIN;

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
    nCurrentTick_         = msg.current_tick;
    nTickDuration_        = msg.tick_duration;
    nTimeFactor_          = msg.time_factor;
    nCheckpointFrequency_ = msg.checkpoint_frequence;
    nSimState_            = msg.etat;
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
    nCurrentTick_ = msg;
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
void SimulationModel::Send( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientControlInformation asn;
    asn().current_tick           = nCurrentTick_;
    asn().tick_duration          = nTickDuration_;
    asn().time_factor            = nTimeFactor_;
    asn().checkpoint_frequence   = nCheckpointFrequency_;
    asn().etat                   = nSimState_;
    asn().send_vision_cones      = bSendVisionCones_;
    asn().profiling_enabled      = bProfilingEnabled_;
    asn.Send( publisher );
}
