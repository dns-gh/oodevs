// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartExercise.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::StartExercise( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, const QString& session )
    : SpawnCommand( parent, config, "simulation_app.exe" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    Start();
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercise::StartExercise( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint )
    : SpawnCommand( parent, config, "simulation_app.exe" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    addArgument( "--checkpoint=" + checkpoint );
    Start();
}

// -----------------------------------------------------------------------------
// Name: StartExercise destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::~StartExercise()
{
    // NOTHING
}
