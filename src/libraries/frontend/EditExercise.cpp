// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "EditExercise.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: EditExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
EditExercise::EditExercise( const tools::GeneralConfig& config, const tools::Path& name )
    : SpawnCommand( config, MakeBinaryName( "preparation_app" ), "preparation" )
{
    AddRootArgument();
    AddExerciseArgument( name );
}

// -----------------------------------------------------------------------------
// Name: EditExercise destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
EditExercise::~EditExercise()
{
    // NOTHING
}
