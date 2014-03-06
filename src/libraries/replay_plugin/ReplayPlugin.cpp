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
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Dispatcher.h"
#include "protocol/MessageParameters.h"
#include "protocol/ReplaySenders.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins::replay;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplayPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::ReplayPlugin( Model_ABC& model, ClientPublisher_ABC& clients,
        dispatcher::LinkResolver_ABC& linkResolver, Loader& loader,
        const ReplayModel_ABC& replayModel )
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
void ReplayPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, const std::string&, Profile_ABC&, unsigned int, bool )
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
            ClientPublisher_ABC& client = linkResolver_.GetConnectedPublisher( endpoint_ );
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
    model_.SendReplayInfo( client, tickNumber_, loader_.GetEndDateTime(), running_ ? sword::running : sword::paused, factor_, loader_.GetFirstTick() );
}

bool ReplayPlugin::HandleClientToReplay( const sword::ClientToReplay& message,
        dispatcher::RewritingPublisher_ABC& unicaster,
        dispatcher::ClientPublisher_ABC& broadcaster )
{
    const auto& msg = message.message();
    if( msg.has_control_pause() )
        Pause( true );
    else if( msg.has_control_resume() )
        Resume( unicaster.GetLink(), msg.control_resume() );
    else if( msg.has_control_change_time_factor() )
        ChangeTimeFactor( msg.control_change_time_factor().time_factor() );
    else if( msg.has_control_skip_to_tick() )
        skipToFrame_ = msg.control_skip_to_tick().tick();
    else if( msg.has_control_skip_to_date() )
        skipToFrame_ = loader_.FindTickForDate( msg.control_skip_to_date().date_time().data() );
    else if( msg.has_time_table_request() )
        RequestTimeTable( msg.time_table_request(), unicaster, broadcaster );
    else if( msg.has_force_refresh_data_request() )
        ReloadAll();
    else
        return false;
    return true;
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
void ReplayPlugin::RequestTimeTable( const sword::TimeTableRequest& msg,
            dispatcher::RewritingPublisher_ABC& unicaster,
            dispatcher::ClientPublisher_ABC& broadcaster )
{
    sword::ReplayToClient reply;
    auto ack = reply.mutable_message()->mutable_time_table_request_ack();
    ack->set_error_code( sword::TimeTableRequestAck::no_error );
    try
    {
        const auto& range = msg.tick_range();
        const int beginTick = range.begin_tick();
        const int endTick = range.end_tick();
        protocol::Check( beginTick > 0, "begin_tick must be greater than zero" );
        protocol::Check( beginTick <= endTick,
                "begin_tick must be equal or less than end_tick" );
        protocol::Check( endTick <= static_cast< int >( loader_.GetTickNumber() ),
                "end_tick must be equal or less than replay tick count" );

        loader_.FillTimeTable( *ack->mutable_time_table(), beginTick, endTick );
        if( msg.broadcast() )
        {
            sword::ReplayToClient table;
            *table.mutable_message()->mutable_time_table() = ack->time_table();
            broadcaster.Send( table );
        }
    }
    catch( const protocol::Exception& e )
    {
        ack->set_error_code( sword::TimeTableRequestAck::invalid_tick_range );
        reply.set_error_msg( tools::GetExceptionMsg( e ));
    }
    unicaster.Send( reply );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::ReloadAll
// Created: MMC 2012-09-14
// -----------------------------------------------------------------------------
void ReplayPlugin::ReloadAll()
{
    loader_.ReloadAll();
}
