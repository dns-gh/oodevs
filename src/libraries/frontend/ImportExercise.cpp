// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ImportExercise.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ImportExercise constructor
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
ImportExercise::ImportExercise( const tools::GeneralConfig& config, const tools::Path& inputScenario, const tools::Path& outputScenario )
    : SpawnCommand( config, "import_app.exe", "import" )
{
    AddRootArgument();
    AddExerciseArgument( outputScenario );
    AddArgument( "import", inputScenario.ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: ImportExercise destructor
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
ImportExercise::~ImportExercise()
{
    // NOTHING
}
