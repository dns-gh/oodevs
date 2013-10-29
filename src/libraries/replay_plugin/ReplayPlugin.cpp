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
#include "dispatcher/Client.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "dispatcher/Loader.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Services.h"
#include "tools/MessageDispatcher_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Dispatcher.h"
#include "protocol/ReplaySenders.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins::replay;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplayPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::ReplayPlugin( Model_ABC& model, ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands,
                            dispatcher::LinkResolver_ABC& linkResolver, Loader& loader, const ReplayModel_ABC& replayModel )
    : model_( model )
    , clients_( clients )
    , linkResolver_( linkResolver )
    , loader_( loader )
    , factor_( 10 )
    , tickNumber_( 0 )
    , running_( false )
    , playingMode_( false )
    , skipToFrame_( -1 )
    , nextPause_( 0 )
    , factory_( new ReplayExtensionFactory( replayModel ) )
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
    services.Declare( sword::service_replay );
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
        SendReplayInfo( clients_ );
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
void ReplayPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, const std::string&, Profile_ABC&, bool )
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
    if( !endpoint_.empty() )
    {
        bool disconnected = false;
        try
        {
            ClientPublisher_ABC& client = linkResolver_.GetPublisher( endpoint_ );
            disconnected = !static_cast< dispatcher::Client& >( client ).HasAnsweredSinceLastTick();
        }
        catch( ... )
        {
            disconnected = true;
        }
        if( disconnected )
        {
            Pause( true );
            if( tickNumber_ != loader_.GetTickNumber() )
                SendReplayInfo( clients_ );
            return;
        }
    }
    if( running_ )
        loader_.Tick();
    if( loader_.GetCurrentTick() >= loader_.GetTickNumber() )
        Pause( true );

    if( running_ || tickNumber_ != loader_.GetTickNumber() )
        SendReplayInfo( clients_ );
    if( nextPause_ > 0 && --nextPause_ == 0 )
        Pause( false );
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
void ReplayPlugin::OnReceive( const std::string& endpoint, const sword::ClientToReplay& wrapper )
{
    if( wrapper.message().has_control_pause() )
        Pause( true );
    else if( wrapper.message().has_control_resume() )
        Resume( endpoint, wrapper.message().control_resume() );
    else if( wrapper.message().has_control_change_time_factor() )
        ChangeTimeFactor( wrapper.message().control_change_time_factor().time_factor() );
    else if( wrapper.message().has_control_skip_to_tick() )
        skipToFrame_ = wrapper.message().control_skip_to_tick().tick();
    else if( wrapper.message().has_control_skip_to_date() )
        skipToFrame_ = loader_.FindTickForDate( wrapper.message().control_skip_to_date().date_time().data() );
    else if( wrapper.message().has_time_table_request() )
        RequestTimeTable( wrapper.message().time_table_request().tick_range() );
    else if( wrapper.message().has_force_refresh_data_request() )
        ReloadAll();
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
void ReplayPlugin::Pause( bool fromClient )
{
    endpoint_.clear();
    sword::ControlAck::ErrorCode error = ( running_ || playingMode_ || !fromClient ) ? sword::ControlAck::no_error: sword::ControlAck::error_already_paused;
    running_ = false;
    if( fromClient )
        playingMode_ = false;
    if( !playingMode_ )
    {
        ::replay::ControlPauseAck asn;
        asn().set_error_code( error );
        asn.Send( clients_ );
    }
    nextPause_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Resume
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::Resume( const std::string& endpoint, const sword::ControlResume& msg )
{
    sword::ControlAck::ErrorCode error = running_ ? sword::ControlAck::error_not_paused : sword::ControlAck::no_error;
    running_ = true;
    if( !playingMode_ )
    {
        ::replay::ControlResumeAck asn;
        asn().set_error_code( error );
        asn.Send( clients_ );
    }
    playingMode_ = !msg.has_tick() || msg.tick() == 0;
    if( playingMode_ )
    {
        endpoint_ = endpoint;
        nextPause_ = 1;
    }
    else
    {
        endpoint_.clear();
        nextPause_ = msg.tick();
    }
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

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::ReloadAll
// Created: MMC 2012-09-14
// -----------------------------------------------------------------------------
void ReplayPlugin::ReloadAll()
{
    loader_.ReloadAll();
}
