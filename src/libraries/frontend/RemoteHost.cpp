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
void RemoteHost::StartSimulation( const std::string& exercise, const std::string& /*session*/ ) const
{
    launcher::ControlStart message;
    message().mutable_exercise()->set_name( exercise );
    message().set_mode( MsgsAdminToLauncher::MsgControlStart::play );
    message().set_use_after_action_analysis( true );
    message().set_use_external_systems( true );
//    message().set_checkpoint( checkpoint );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StartReplay
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
void RemoteHost::StartReplay( const std::string& exercise, const std::string& /*session*/ ) const
{
    launcher::ControlStart message;
    message().mutable_exercise()->set_name( exercise );
    message().set_mode( MsgsAdminToLauncher::MsgControlStart::replay );
    message().set_use_after_action_analysis( true );
    message().set_use_external_systems( true );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::StopSession
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::StopSession( const std::string& exercise, const std::string& /*session*/ ) const
{
    launcher::ControlStop message;
    message().mutable_exercise()->set_name( exercise );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const MsgsLauncherToAdmin::MsgExercicesListResponse& message )
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
void RemoteHost::Handle( const MsgsLauncherToAdmin::MsgControlStartAck& message )
{
    boost::shared_ptr< Exercise_ABC > exercise( exercises_[ message.exercise().name() ] );
    if( !exercise.get() )
        exercise.reset( new RemoteExercise( *this, *this, message.exercise(), controller_ ) );
    exercise->SetRunning( message.exercise().running() );
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const MsgsAuthenticationToClient::MsgProfileDescriptionList& message )
{
    // $$$$ SBO 2010-11-22: TODO, handle profile list
}

// -----------------------------------------------------------------------------
// Name: RemoteHost::Handle
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteHost::Handle( const MsgsLauncherToAdmin::MsgControlStopAck& message )
{
    std::map< std::string, boost::shared_ptr< Exercise_ABC > >::iterator it = exercises_.find( message.exercise().name() );
    if( it != exercises_.end() )
        it->second->SetRunning( message.exercise().running() );
}
