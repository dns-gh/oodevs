// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReplayPlugin.h"
#include "ReplayExtensionFactory.h"
#include "dispatcher/Loader.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Services.h"
#include "tools/MessageDispatcher_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ReplaySenders.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins::replay;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplayPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::ReplayPlugin( Model_ABC& model, ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands,
                            Loader& loader, const ReplayModel_ABC& replayModel )
    : model_      ( model )
    , clients_    ( clients )
    , loader_     ( loader )
    , factor_     ( 1 )
    , tickNumber_ ( 0 )
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
void ReplayPlugin::Register( Services& services )
{
    services.Declare< ::replay::Service >();
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
void ReplayPlugin::Receive( const sword::SimToClient& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, const std::string&, Profile_ABC& )
{
    model_.Send( client );
    SendReplayInfo( client );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::OnTimer
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::OnTimer()
{
    if( running_ )
        loader_.Tick();
    if( running_ || tickNumber_ != loader_.GetTickNumber() )
        SendReplayInfo( clients_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::SendReplayInfo
// Created: AGE 2007-11-28
// -----------------------------------------------------------------------------
void ReplayPlugin::SendReplayInfo( ClientPublisher_ABC& client )
{
    tickNumber_ = loader_.GetTickNumber();
    model_.SendReplayInfo( client, tickNumber_, running_ ? sword::running : sword::paused, factor_, loader_.GetFirstTick() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::OnReceive( const std::string& , const sword::ClientToReplay& wrapper )
{
    if( wrapper.message().has_control_pause() )
        Pause();
    else if( wrapper.message().has_control_resume() )
        Resume();
    else if( wrapper.message().has_control_change_time_factor() )
        ChangeTimeFactor( wrapper.message().control_change_time_factor().time_factor() );
    else if( wrapper.message().has_control_skip_to_tick() )
        skipToFrame_ = wrapper.message().control_skip_to_tick().tick();
    else if( wrapper.message().has_time_table_request() )
        RequestTimeTable( wrapper.message().time_table_request().tick_range() );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::ChangeTimeFactor
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::ChangeTimeFactor( unsigned int factor )
{
    ::replay::ControlChangeTimeFactorAck message;
    if( factor )
    {
        factor_ = factor;
        MT_Timer_ABC::Start( 10000 / factor );
        message().set_error_code( sword::ControlAck::no_error );
    }
    else
        message().set_error_code( sword::ControlAck::error_invalid_time_factor );
    message().set_time_factor( factor_ );
    message.Send( clients_ );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Pause
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Pause()
{
    ::replay::ControlPauseAck asn;
    asn().set_error_code( running_ ? sword::ControlAck::no_error: sword::ControlAck::error_already_paused );
    asn.Send( clients_ );
    running_ = false;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Resume
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Resume()
{
    ::replay::ControlResumeAck asn;
    asn().set_error_code( running_ ? sword::ControlAck::error_not_paused : sword::ControlAck::no_error );
    asn.Send( clients_ );
    running_ = true;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::SkipToFrame
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::SkipToFrame( unsigned int frame )
{
    ::replay::ControlSkipToTickAck asn;
    asn().set_tick( loader_.GetCurrentTick() );
    if( frame < loader_.GetTickNumber() )
    {
        asn().set_tick( frame );
        asn().set_error_code( sword::ControlAck::no_error );
        MT_LOG_INFO_MSG( "Skipping to frame " << frame );
    }
    else
        asn().set_error_code( sword::ControlAck::error_invalid_time_factor );
    asn.Send( clients_ );
    if( frame < loader_.GetTickNumber() )
        loader_.SkipToFrame( frame );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::RequestTimeTable
// Created: JSR 2011-07-22
// -----------------------------------------------------------------------------
void ReplayPlugin::RequestTimeTable( const sword::TimeTableRequest_TimeRange& msg )
{
    ::replay::TimeTableRequestAck ack;
    int beginTick = msg.begin_tick();
    int endTick = msg.end_tick();
    bool valid = beginTick > 0 && beginTick <= endTick && endTick <= static_cast< int >( loader_.GetTickNumber() );
    if( valid )
        ack().set_error_code( sword::TimeTableRequestAck::no_error );
    else
        ack().set_error_code( sword::TimeTableRequestAck::invalid_tick_range );
    ack.Send( clients_ );
    if( valid )
    {
        ::replay::TimeTable asn;
        loader_.FillTimeTable( asn(), beginTick, endTick );
        asn.Send( clients_ );
    }
}
