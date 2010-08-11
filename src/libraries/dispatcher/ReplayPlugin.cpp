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
#include "protocol/ClientPublisher_ABC.h"
#include "Model.h"
#include "ReplayExtensionFactory.h"

#include <MT/MT_Logger/MT_Logger_lib.h>
#include "tools/MessageDispatcher_ABC.h"
#include "Services.h"

//#include "protocol/protocol.h"
#include "protocol/ReplaySenders.h"
#include "protocol/ClientSenders.h"
#include "protocol/Simulation.h"


using namespace dispatcher;


// -----------------------------------------------------------------------------
// Name: ReplayPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::ReplayPlugin( Model& model, ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands, Loader& loader, const ReplayModel_ABC& replayModel )
    : model_      ( model )
    , clients_    ( clients )
    , loader_     ( loader )
    , factor_     ( 1 )
    , running_    ( false )
    , skipToFrame_( -1 )
    , factory_    ( new ReplayExtensionFactory( replayModel ) )
{
    model.RegisterFactory( *factory_ );
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
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Register
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void ReplayPlugin::Register( dispatcher::Services& services )
{
    services.Declare< replay::Service >();
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
void ReplayPlugin::Receive( const MsgsSimToClient::MsgSimToClient& )
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
    model_.SendReplayInfo( client, loader_.GetTickNumber(), running_ ? Common::running : Common::paused, factor_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::OnReceive( const std::string& , const MsgsClientToReplay::MsgClientToReplay& wrapper )
{
    if( wrapper.message().has_control_pause() )
        Pause();
    else if( wrapper.message().has_control_resume() )
        Resume();
    else if( wrapper.message().has_control_change_time_factor() )
        ChangeTimeFactor( wrapper.message().control_change_time_factor().time_factor() );
    else if( wrapper.message().has_control_skip_to_tick() )
        skipToFrame_ = wrapper.message().control_skip_to_tick().tick();
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::ChangeTimeFactor
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::ChangeTimeFactor( unsigned factor )
{
    replay::ControlChangeTimeFactorAck message;
    if( factor )
    {
        factor_ = factor;
        MT_Timer_ABC::Start( MT_TimeSpan( (int)( 10000 / factor ) ) );
        message().set_error_code( MsgsSimToClient::ControlAck_ErrorCode_no_error);
    }
    else
        message().set_error_code( MsgsSimToClient::ControlAck_ErrorCode_error_invalid_time_factor );

    message().set_time_factor( factor_ );
    message.Send( clients_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Pause
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Pause()
{
    replay::ControlPauseAck asn;
    asn().set_error_code( running_ ? MsgsSimToClient::ControlAck_ErrorCode_no_error: MsgsSimToClient::ControlAck_ErrorCode_error_already_paused );
    asn.Send( clients_ );
    running_ = false;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Resume
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Resume()
{
    replay::ControlResumeAck asn;
    asn().set_error_code( running_ ? MsgsSimToClient::ControlAck_ErrorCode_error_not_paused :  MsgsSimToClient::ControlAck_ErrorCode_no_error );
    asn.Send( clients_ );
    running_ = true;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::SkipToFrame
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::SkipToFrame( unsigned frame )
{
    replay::ControlSkipToTickAck asn;
    asn().set_tick( loader_.GetCurrentTick() );
    if( frame < loader_.GetTickNumber() )
    {
        asn().set_tick( frame );
        asn().set_error_code( MsgsSimToClient::ControlAck_ErrorCode_no_error );
        MT_LOG_INFO_MSG( "Skipping to frame " << frame );
    }
    else
        asn().set_error_code( MsgsSimToClient::ControlAck_ErrorCode_error_invalid_time_factor );
    asn.Send( clients_ );
    if( frame < loader_.GetTickNumber() )
        loader_.SkipToFrame( frame );
}
