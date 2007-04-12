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
#include "ClientsNetworker.h"
#include "network_def.h"
#include "game_asn/Asn.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LoaderFacade constructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
LoaderFacade::LoaderFacade( ClientsNetworker& clients, SimulationDispatcher& simulation, const std::string& directory )
    : clients_( clients )
    , loader_( new Loader( simulation, directory ) )
    , factor_( 1 )
    , running_( false )
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
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::OnReceive( const ASN1T_MsgsOutClient& asnInMsg )
{
    switch( asnInMsg.msg.t )
    {
    case T_MsgsOutClient_msg_msg_ctrl_pause:
        TogglePause( true );
        break;
    case T_MsgsOutClient_msg_msg_ctrl_resume:
        TogglePause( false );
        break;
    case T_MsgsOutClient_msg_msg_ctrl_change_time_factor:
        ChangeTimeFactor( asnInMsg.msg.u.msg_ctrl_change_time_factor );
        break;
    case T_MsgsOutClient_msg_msg_ctrl_skip_to_tick:
        SkipToFrame( asnInMsg.msg.u.msg_ctrl_skip_to_tick ); 
        break;
    };
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::ChangeTimeFactor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::ChangeTimeFactor( unsigned factor )
{
    ASN1T_MsgsInClient message;
    message.context = 0;
    message.msg.t = T_MsgsInClient_msg_msg_ctrl_change_time_factor_ack;
    ASN1T_MsgCtrlChangeTimeFactorAck ack;

    if( factor )
    {
        factor_ = factor;
        MT_Timer_ABC::Start( MT_TimeSpan( (int)( 10000 / factor ) ) );

        ack.error_code = EnumCtrlErrorCode::no_error;
        message.msg.u.msg_ctrl_change_time_factor_ack = &ack;
    }
    else
        ack.error_code = EnumCtrlErrorCode::error_invalid_time_factor;
    ack.time_factor = factor_;
    clients_.Send( message );
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::TogglePause
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::TogglePause( bool pause )
{
    ASN1T_MsgsInClient message;
    message.context = 0;
    if( pause )
    {
        message.msg.t = T_MsgsInClient_msg_msg_ctrl_pause_ack;
        message.msg.u.msg_ctrl_pause_ack = running_ ? EnumCtrlErrorCode::no_error
                                                    : EnumCtrlErrorCode::error_already_paused;
    }
    else
    {
        message.msg.t = T_MsgsInClient_msg_msg_ctrl_resume_ack;
        message.msg.u.msg_ctrl_resume_ack = running_ ? EnumCtrlErrorCode::error_not_paused
                                                    :  EnumCtrlErrorCode::no_error;
    }
    running_ = !pause;
    clients_.Send( message );
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::SkipToFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::SkipToFrame( unsigned frame )
{
    ASN1T_MsgsInClient message;
    message.context = 0;
    message.msg.t = T_MsgsInClient_msg_msg_ctrl_skip_to_tick_ack;
    ASN1T_MsgCtrlSkipToTickAck ack;
    message.msg.u.msg_ctrl_skip_to_tick_ack = &ack;

    ack.tick = loader_->GetCurrentTick();
    if( frame < loader_->GetTickNumber() )
    {
        ack.tick = frame;    
        ack.error_code = EnumCtrlErrorCode::no_error;
    }
    else
        ack.error_code = EnumCtrlErrorCode::error_invalid_time_factor;
    clients_.Send( message );
    if( frame < loader_->GetTickNumber() )
        loader_->SkipToFrame( frame );
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::Send
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::Send( Publisher_ABC& publisher ) const
{
    AsnMsgInClientCtrlReplayInfo message;
    message().current_tick  = loader_->GetCurrentTick();
    message().tick_duration = 10; // $$$$ AGE 2007-04-11: 
    message().time_factor = factor_;
    message().etat = running_ ? EnumEtatSim::running : EnumEtatSim::paused;
    message().tick_count = loader_->GetTickNumber();

    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::Update
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::Update()
{
    manager_.Update();
}

// -----------------------------------------------------------------------------
// Name: LoaderFacade::OnTimer
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void LoaderFacade::OnTimer()
{
    if( running_ )
        loader_->Tick();
}
