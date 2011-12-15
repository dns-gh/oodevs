// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "AttachCommand.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: AttachCommand constructor
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
AttachCommand::AttachCommand( const tools::GeneralConfig& config, unsigned long processId, bool attach,
                              const std::string& exercise, const std::string& session )
    : SpawnCommand( config, processId, attach )
    , exercise_( exercise )
    , session_ ( session )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttachCommand destructor
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
AttachCommand::~AttachCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttachCommand::GetExercise
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
std::string AttachCommand::GetExercise() const
{
    return exercise_;
}

// -----------------------------------------------------------------------------
// Name: AttachCommand::GetSession
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
std::string AttachCommand::GetSession() const
{
    return session_;
}

// -----------------------------------------------------------------------------
// Name: AttachCommand::GetStartedExercise
// Created: JSR 2011-12-14
// -----------------------------------------------------------------------------
std::string AttachCommand::GetStartedExercise() const
{
    return exercise_;
}
