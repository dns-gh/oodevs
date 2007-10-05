// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "JoinExercise.h"

// -----------------------------------------------------------------------------
// Name: JoinExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercise::JoinExercise( QObject* parent, const tools::GeneralConfig& config, const QString& exercise )
    : SpawnCommand( parent, config, "gaming_app.exe" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    addArgument( "--host=localhost:10001" ); // $$$$ AGE 2007-10-05: 
    start();
}

// -----------------------------------------------------------------------------
// Name: JoinExercise destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercise::~JoinExercise()
{
    // NOTHING
}
