// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ReplayPlugin.h"
#include "Loader.h"
#include "ClientPublisher_ABC.h"
#include "network_def.h"
#include "game_asn/Asn.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "tools/MessageDispatcher_ABC.h"
#include "Model.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplayPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::ReplayPlugin( Model& model, ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands, Loader& loader )
    : model_      ( model )
    , clients_    ( clients )
    , loader_     ( loader )
    , factor_     ( 1 )
    , running_    ( false )
    , skipToFrame_( -1 )
{
    clientCommands.RegisterMessage( *this, &ReplayPlugin::OnReceive );
    ChangeTimeFactor( factor_ );
    manager_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::~ReplayPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Update
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Update()
{
    if( skipToFrame_ >= 0 )
    {
        SkipToFrame( skipToFrame_ );
        skipToFrame_ = -1;
    }
    manager_.Update();
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Receive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::Receive( const ASN1T_MsgsSimToClient& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& )
{
    model_.Send( client );
    SendReplayInfo( client );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::NotifyClientLeft( ClientPublisher_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::OnTimer
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::OnTimer()
{
    if( running_ )
    {
        loader_.Tick();
        SendReplayInfo( clients_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::SendReplayInfo
// Created: AGE 2007-11-28
// -----------------------------------------------------------------------------
void ReplayPlugin::SendReplayInfo( ClientPublisher_ABC& client )
{
    model_.SendReplayInfo( client, loader_.GetTickNumber(), running_ ? EnumSimulationState::running : EnumSimulationState::paused, factor_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::OnReceive( const std::string& , const ASN1T_MsgsClientToReplay& asnMsg )
{
    switch( asnMsg.msg.t )
    {
        case T_MsgsClientToReplay_msg_msg_control_pause:
            Pause();
            break;
        case T_MsgsClientToReplay_msg_msg_control_resume:
            Resume();
            break;
        case T_MsgsClientToReplay_msg_msg_control_change_time_factor:
            ChangeTimeFactor( asnMsg.msg.u.msg_control_change_time_factor );
            break;
        case T_MsgsClientToReplay_msg_msg_control_skip_to_tick:
            skipToFrame_ = asnMsg.msg.u.msg_control_skip_to_tick;
            break;
    };
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::ChangeTimeFactor
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::ChangeTimeFactor( unsigned factor )
{
    AsnMsgReplayToClientControlChangeTimeFactorAck asn;
    if( factor )
    {
        factor_ = factor;
        MT_Timer_ABC::Start( MT_TimeSpan( (int)( 10000 / factor ) ) );
        asn().error_code = EnumControlErrorCode::no_error;
    }
    else
        asn().error_code = EnumControlErrorCode::error_invalid_time_factor;

    asn().time_factor = factor_;
    asn.Send( clients_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Pause
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Pause()
{
    AsnMsgReplayToClientControlPauseAck asn;
    asn() = running_ ? EnumControlErrorCode::no_error : EnumControlErrorCode::error_already_paused;
    asn.Send( clients_ );
    running_ = false;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Resume
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Resume()
{
    AsnMsgReplayToClientControlResumeAck asn;
    asn() = running_ ? EnumControlErrorCode::error_not_paused :  EnumControlErrorCode::no_error;
    asn.Send( clients_ );
    running_ = true;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::SkipToFrame
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::SkipToFrame( unsigned frame )
{
    AsnMsgReplayToClientControlSkipToTickAck asn;
    asn().tick = loader_.GetCurrentTick();
    if( frame < loader_.GetTickNumber() )
    {
        asn().tick = frame;    
        asn().error_code = EnumControlErrorCode::no_error;
        MT_LOG_INFO_MSG( "Skipping to frame " << frame );
    }
    else
        asn().error_code = EnumControlErrorCode::error_invalid_time_factor;
    asn.Send( clients_ );
    if( frame < loader_.GetTickNumber() )
        loader_.SkipToFrame( frame );
}
