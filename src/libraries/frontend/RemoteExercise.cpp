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
#include <boost/algorithm/string.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: RemoteExercise constructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::RemoteExercise( const Host_ABC& host, const ExerciseIdentifierFactory_ABC& factory,
                                const tools::Path& exercise, kernel::Controller& controller )
    : controller_( controller )
    , host_      ( host )
    , name_      ( exercise )
    , id_        ( factory.CreateIdentifier( exercise ) )
    , port_      ( 0 )
    , running_   ( false )
{
    controller_.Create( static_cast< Exercise_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise destructor
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
RemoteExercise::~RemoteExercise()
{
    controller_.Delete( static_cast< Exercise_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::GetId
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
const std::string& RemoteExercise::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::GetName
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
const tools::Path& RemoteExercise::GetName() const
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
    return boost::algorithm::starts_with( GetId(), host ); // $$$$ SBO 2010-10-22: TODO, something better (resolve host)
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Start
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteExercise::Start( const tools::Path& session ) const
{
    host_.StartSimulation( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::StartDispatcher
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
void RemoteExercise::StartDispatcher( const tools::Path& session, const T_Parameters& parameters ) const
{
    host_.StartDispatcher( name_, session, parameters );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Start
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
void RemoteExercise::Replay( const tools::Path& session ) const
{
    host_.StartReplay( name_, session );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::Stop
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void RemoteExercise::Stop( const tools::Path& session ) const
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
    controller_.Update( static_cast< Exercise_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteExercise::QueryProfileList
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
void RemoteExercise::QueryProfileList() const
{
    host_.QueryProfileList( name_ );
}

