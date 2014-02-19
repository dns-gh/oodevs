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
    : publisher_ ( publisher )
    , id_        ( host )
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
std::string RemoteHost::CreateIdentifier( const tools::Path& exercise ) const
{
    return id_ + "/" + exercise.ToUTF8();
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StartSimulation
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::StartSimulation( const tools::Path& exercise, const tools::Path& session ) const
{
    launcher::SessionStartRequest message;
    message().set_exercise( exercise.ToUTF8() );
    message().set_session( session.ToUTF8() );
    message().set_type( sword::SessionStartRequest::simulation );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StartDispatcher
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
void RemoteHost::StartDispatcher( const tools::Path& exercise, const tools::Path& session, const T_Parameters& parameters ) const
{
    launcher::SessionStartRequest message;
    message().set_exercise( exercise.ToUTF8() );
    message().set_session( session.ToUTF8() );
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
// Name: RemoteHost::StopSession
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::StopSession( const tools::Path& exercise, const tools::Path& session ) const
{
    launcher::SessionStopRequest message;
    message().set_exercise( exercise.ToUTF8() );
    message().set_session( session.ToUTF8() );
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
        boost::shared_ptr< RemoteExercise > exercise( new RemoteExercise( *this, *this, tools::Path::FromUTF8( message.exercise( i ) ), controller_ ) );
        exercises_[ exercise->GetName() ] = exercise;
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const sword::SessionStartResponse& message )
{
    tools::Path exercisePath = tools::Path::FromUTF8( message.exercise() );
    boost::shared_ptr< Exercise_ABC > exercise( exercises_[ exercisePath ] );
    if( !exercise.get() )
        exercise.reset( new RemoteExercise( *this, *this, exercisePath, controller_ ) );
    exercise->SetRunning( true );
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
    auto it = exercises_.find( tools::Path::FromUTF8( message.exercise() ) );
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
    auto it = exercises_.find( tools::Path::FromUTF8( message.exercise() ) );
    if( it != exercises_.end() )
        it->second->SetRunning( false );
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
