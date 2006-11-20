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
    , nExerciseID_         ( 0 )
    , nCheckpointFrequency_( 0 )
    , nSimState_           ( EnumEtatSim::stopped )
    , bSendVisionCones_    ( false )
    , bProfilingEnabled_   ( false )
    , strConfigPath_       ()
{

}

// -----------------------------------------------------------------------------
// Name: SimulationModel destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SimulationModel::~SimulationModel()
{

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
void SimulationModel::Update( const ASN1T_MsgCtrlInfo& msg )
{
    nCurrentTick_         = msg.current_tick;
    nTickDuration_        = msg.tick_duration;
    nTimeFactor_          = msg.time_factor;
    nExerciseID_          = msg.id_exercice;
    nCheckpointFrequency_ = msg.checkpoint_frequence;
    nSimState_            = msg.etat;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Stop
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Stop( const ASN1T_MsgCtrlStopAck& msg )
{
    if( msg == EnumCtrlErrorCode::no_error )
        nSimState_ = EnumEtatSim::stopped;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Pause
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Pause( const ASN1T_MsgCtrlPauseAck& msg )
{
    if( msg == EnumCtrlErrorCode::no_error )
        nSimState_ = EnumEtatSim::paused;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_Resume
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update_Resume( const ASN1T_MsgCtrlResumeAck& msg )
{
    if( msg == EnumCtrlErrorCode::no_error )
        nSimState_ = EnumEtatSim::running;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Update( const ASN1T_MsgCtrlChangeTimeFactorAck& msg )
{
    if( msg.error_code == EnumCtrlErrorCode::no_error )
        nTimeFactor_ = msg.time_factor;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const ASN1T_MsgCtrlBeginTick& msg )
{
    nCurrentTick_ = msg;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
void SimulationModel::Update( const ASN1T_MsgCtrlEndTick& msg )
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
//void SimulationModel::Update( const ASN1T_MsgCtrlCheckPointSetFrequencyAck& msg )
//{
//    nCheckpointFrequency_ = msg;
//}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Update_MsgInit
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SimulationModel::Update_MsgInit( DIN::DIN_Input& msg )
{
    msg >> bSendVisionCones_
        >> bProfilingEnabled_
        >> strConfigPath_;
}

// -----------------------------------------------------------------------------
// Name: SimulationModel::Send
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SimulationModel::Send( Publisher_ABC& publisher ) const
{
    AsnMsgInClientCtrlInfo asn;
    asn().current_tick         = nCurrentTick_;
    asn().tick_duration        = nTickDuration_;
    asn().time_factor          = nTimeFactor_;
    asn().id_exercice          = nExerciseID_;
    asn().checkpoint_frequence = nCheckpointFrequency_;
    asn().etat                 = nSimState_;
    asn.Send( publisher );


    DIN_BufferedMessage msg = publisher.GetDinMsg();
    msg << bSendVisionCones_
        << bProfilingEnabled_
        << strConfigPath_;
    publisher.Send( eMsgInit, msg );
}
