// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "RemoteExercise.h"
#include "ExerciseIdentifierFactory_ABC.h"
#include "Host_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"
#include "protocol/LauncherSenders.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: RemoteExercise constructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::RemoteExercise( const Host_ABC& host, const ExerciseIdentifierFactory_ABC& factory, const sword::Exercise& message, kernel::Controller& controller )
    : controller_( controller )
    , host_      ( host )
    , name_      ( message.name() )
    , id_        ( factory.CreateIdentifier( name_ ) )
    , port_      ( message.has_port() ? message.port() : 0 )
    , running_   ( message.has_running() ? message.running() : false )
{
    controller_.Create( *(Exercise_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise constructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::RemoteExercise( const Host_ABC& host, const ExerciseIdentifierFactory_ABC& factory, const std::string& exercise, kernel::Controller& controller )
    : controller_( controller )
    , host_      ( host )
    , name_      ( exercise )
    , id_        ( factory.CreateIdentifier( name_ ) )
    , port_      ( 0 )
    , running_   ( false )
{
    controller_.Create( *(Exercise_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise destructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::~RemoteExercise()
{
    controller_.Delete( *(Exercise_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::GetId
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
std::string RemoteExercise::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::GetName
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
std::string RemoteExercise::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::GetPort
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
unsigned int RemoteExercise::GetPort() const
{
    return port_;
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::IsRunning
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
bool RemoteExercise::IsRunning() const
{
    return running_;
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::IsHostedBy
// Created: SBO 2010-10-22
// -----------------------------------------------------------------------------
bool RemoteExercise::IsHostedBy( const std::string& host ) const
{
    return QString( GetId().c_str() ).startsWith( host.c_str() ); // $$$$ SBO 2010-10-22: TODO, something better (resolve host)
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Start
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteExercise::Start( const std::string& session ) const
{
    host_.StartSimulation( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::StartDispatcher
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
void RemoteExercise::StartDispatcher( const std::string& session ) const
{
    host_.StartDispatcher( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Start
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
void RemoteExercise::Replay( const std::string& session ) const
{
    host_.StartReplay( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Stop
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteExercise::Stop( const std::string& session ) const
{
    host_.StopSession( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::SetRunning
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
void RemoteExercise::SetRunning( bool running )
{
    running_ = running;
    controller_.Update( *(Exercise_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::QueryProfileList
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteExercise::QueryProfileList() const
{
    host_.QueryProfileList( name_ );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Pause
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteExercise::Pause(const std::string& session) const
{
    host_.Pause( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Resume
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteExercise::Resume(const std::string& session) const
{
    host_.Resume( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::SaveCheckpoint
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteExercise::SaveCheckpoint(const std::string& session, const std::string& name) const
{
    host_.SaveCheckpoint( name_, session, name );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::QueryConnectedProfileList
// Created: AHC 2010-05-23
// -----------------------------------------------------------------------------
void RemoteExercise::QueryConnectedProfileList(const std::string& session ) const
{
    host_.QueryConnectedProfileList( name_, session );
}
