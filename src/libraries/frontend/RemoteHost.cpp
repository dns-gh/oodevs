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
