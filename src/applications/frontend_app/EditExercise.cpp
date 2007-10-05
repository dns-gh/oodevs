// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "EditExercise.h"

// -----------------------------------------------------------------------------
// Name: EditExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
EditExercise::EditExercise( QObject* parent, const tools::GeneralConfig& config, const QString& name )
    : SpawnCommand( parent, config, "preparation_app.exe" )
{
    AddRootDirArgument();
    AddExerciseArgument( name );
    Start();
}

// -----------------------------------------------------------------------------
// Name: EditExercise destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
EditExercise::~EditExercise()
{
    // NOTHING
}
