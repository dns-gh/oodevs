// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LoaderFacade.h"
#include "Loader.h"
#include "ClientPublisher_ABC.h"
#include "network_def.h"
#include "game_asn/Asn.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LoaderFacade constructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
LoaderFacade::LoaderFacade( ClientPublisher_ABC& clients, Loader& loader )
    : clients_    ( clients )
    , loader_     ( loader )
    , factor_     ( 1 )
    , running_    ( false )
    , skipToFrame_( -1 )
{
    ChangeTimeFactor( factor_ );
    manager_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade destructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
LoaderFacade::~LoaderFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::OnReceive
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void LoaderFacade::OnReceive( const ASN1T_MsgsClientToReplay& asnMsg )
{
    switch( asnMsg.msg.t )
    {
        case T_MsgsClientToReplay_msg_msg_control_pause:
            TogglePause( true );
            break;
        case T_MsgsClientToReplay_msg_msg_control_resume:
            TogglePause( false );
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
// Name: LoaderFacade::ChangeTimeFactor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::ChangeTimeFactor( unsigned factor )
{
    // $$$$ NLD 2007-04-24: A CHANGER => MiddleToClient
    AsnMsgSimToClientControlChangeTimeFactorAck asn;

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
// Name: LoaderFacade::TogglePause
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::TogglePause( bool pause )
{
    if( pause )
    {
        AsnMsgSimToClientControlPauseAck asn;
        asn() = running_ ? EnumControlErrorCode::no_error : EnumControlErrorCode::error_already_paused;
        asn.Send( clients_ );
    }
    else
    {
        AsnMsgSimToClientControlResumeAck asn;
        asn() = running_ ? EnumControlErrorCode::error_not_paused :  EnumControlErrorCode::no_error;
        asn.Send( clients_ );
    }
    running_ = !pause;
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::SkipToFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::SkipToFrame( unsigned frame )
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

// -----------------------------------------------------------------------------
// Name: LoaderFacade::SendReplayInfo
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::SendReplayInfo( ClientPublisher_ABC& publisher ) const
{
    AsnMsgReplayToClientControlReplayInformation asn;
    asn().current_tick  = loader_.GetCurrentTick();
    asn().tick_duration = 10; // $$$$ AGE 2007-04-11: 
    asn().time_factor = factor_;
    asn().status = running_ ? EnumSimulationState::running : EnumSimulationState::paused;
    asn().tick_count = loader_.GetTickNumber();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::Update
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::Update()
{
    if( skipToFrame_ >= 0 )
    {
        SkipToFrame( skipToFrame_ );
        skipToFrame_ = -1;
    }
    manager_.Update();
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::OnTimer
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::OnTimer()
{
    if( running_ )
        loader_.Tick();
}
