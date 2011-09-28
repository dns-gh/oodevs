// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "RemoteHost.h"
#include "LauncherPublisher.h"
#include "RemoteExercise.h"
#include "clients_kernel/Controller.h"
#include "protocol/LauncherSenders.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: RemoteHost constructor
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
RemoteHost::RemoteHost( LauncherPublisher& publisher, const std::string& host, kernel::Controller& controller )
    : publisher_( publisher )
    , id_( host )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost destructor
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
RemoteHost::~RemoteHost()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::GetId
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
std::string RemoteHost::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::CreateIdentifier
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
std::string RemoteHost::CreateIdentifier( const std::string& exercise ) const
{
    return id_ + "/" + exercise;
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StartSimulation
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::StartSimulation( const std::string& exercise, const std::string& session ) const
{
    launcher::SessionStartRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().set_type( sword::SessionStartRequest::simulation );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StartDispatcher
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
void RemoteHost::StartDispatcher( const std::string& exercise, const std::string& session, const T_Parameters& parameters ) const
{
    launcher::SessionStartRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().set_type( sword::SessionStartRequest::dispatch );
    for( T_Parameters::const_iterator it = parameters.begin(); it != parameters.end(); ++it )
    {
        sword::SessionParameter* pParameter = message().add_parameter();
        pParameter->set_key( it->first );
        pParameter->set_value( it->second );
    }
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StartReplay
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
void RemoteHost::StartReplay( const std::string& exercise, const std::string& session ) const
{
    launcher::SessionStartRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().set_type( sword::SessionStartRequest::replay );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StopSession
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::StopSession( const std::string& exercise, const std::string& session ) const
{
    launcher::SessionStopRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::QueryProfileList
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void RemoteHost::QueryProfileList(const std::string& exercise) const
{
    launcher::ProfileListRequest message;
    message().set_exercise( exercise );
    message.Send( publisher_ );
 }

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::ExerciseListResponse& message )
{
    exercises_.clear();
    for( int i = 0; i < message.exercise().size(); ++i )
    {
        boost::shared_ptr< RemoteExercise > exercise( new RemoteExercise( *this, *this, message.exercise( i ), controller_ ) );
        exercises_[ exercise->GetName() ] = exercise;
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionStartResponse& message )
{
    boost::shared_ptr< Exercise_ABC > exercise( exercises_[ message.exercise() ] );
    if( !exercise.get() )
        exercise.reset( new RemoteExercise( *this, *this, message.exercise(), controller_ ) );
    exercise->SetRunning( true );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::ProfileListResponse& /*message*/ )
{
    // $$$$ SBO 2010-11-22: TODO, handle profile list
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionCommandExecutionResponse& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionNotification& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionParameterChangeResponse& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionStatus& message )
{
    std::map< std::string, boost::shared_ptr< Exercise_ABC > >::iterator it = exercises_.find( message.exercise() );
    if( it != exercises_.end() && message.status() == sword::SessionStatus::running )
        it->second->SetRunning( true );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: AHC 2011-05-30
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionListResponse& /*message*/ )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionStopResponse& message )
{
    std::map< std::string, boost::shared_ptr< Exercise_ABC > >::iterator it = exercises_.find( message.exercise() );
    if( it != exercises_.end() )
        it->second->SetRunning( false );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: AHC 2011-05-24
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::ConnectedProfileListResponse& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: AHC 2011-05-30
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::CheckpointListResponse& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: AHC 2011-05-30
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::CheckpointDeleteResponse& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Pause
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteHost::Pause(const std::string& exercise, const std::string& session) const
{
    launcher::SessionCommandExecutionRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().set_set_running( false );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Resume
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteHost::Resume(const std::string& exercise, const std::string& session) const
{
    launcher::SessionCommandExecutionRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().set_set_running( true );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::SaveCheckpoint
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteHost::SaveCheckpoint(const std::string& exercise, const std::string& session, const std::string& name) const
{
    launcher::SessionCommandExecutionRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().set_save_checkpoint( name );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::ChangeDateTime
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
void RemoteHost::ChangeDateTime( const std::string& exercise, const std::string& session, const std::string& date ) const
{
    launcher::SessionCommandExecutionRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message().mutable_time_change()->set_data( date );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::QueryConnectedProfileList
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteHost::QueryConnectedProfileList( const std::string& exercise, const std::string& session) const
{
    launcher::ConnectedProfileListRequest message;
    message().set_exercise( exercise );
    message().set_session( session );
    message.Send( publisher_ );
}
