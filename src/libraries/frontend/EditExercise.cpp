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
#include "DebugConfig.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: EditExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
EditExercise::EditExercise( const tools::GeneralConfig& config, const tools::Path& name, const DebugConfig& debug )
    : SpawnCommand( config, MakeBinaryName( "preparation_app" ), "preparation" )
{
    AddRootArgument();
    AddExerciseArgument( name );
    if( debug.gaming.hasMapnik )
        AddArgument( "--mapnik" );
}

// -----------------------------------------------------------------------------
// Name: EditExercise destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
EditExercise::~EditExercise()
{
    // NOTHING
}
