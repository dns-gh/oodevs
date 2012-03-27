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
AttachCommand::AttachCommand( const tools::GeneralConfig& config, unsigned long processId, bool attach )
    : SpawnCommand( config, processId, attach )
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
