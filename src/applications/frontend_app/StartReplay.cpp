// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "StartReplay.h"

// -----------------------------------------------------------------------------
// Name: StartReplay constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::StartReplay( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, const QString& replay )
    : SpawnCommand( parent, config, "replayer_app.exe" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    addArgument( "--record=\"" + replay + "\"" );
    Start();
}

// -----------------------------------------------------------------------------
// Name: StartReplay destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::~StartReplay()
{
    // NOTHING
}
