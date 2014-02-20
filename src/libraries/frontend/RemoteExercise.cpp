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

