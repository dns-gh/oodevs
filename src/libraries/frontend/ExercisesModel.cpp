// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ExercisesModel.h"
#include "ExerciseFactory_ABC.h"
#include "Exercise.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ExercisesModel constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
ExercisesModel::ExercisesModel( ExerciseFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExercisesModel destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
ExercisesModel::~ExercisesModel()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ExercisesModel::CreateExercise
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void ExercisesModel::CreateExercise( const ASN1T_MsgExerciseCreation& message )
{
    std::auto_ptr< Exercise > exercise( factory_.Create( message ) );
    Register( exercise->GetName(), *exercise.release() );
}
