// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ExerciseFactory.h"
#include "Exercise.h"
#include "Model.h"
#include "TerrainsModel.h"
#include "DatasetsModel.h"
#include "PhysicalModelsModel.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ExerciseFactory constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
ExerciseFactory::ExerciseFactory( kernel::Controllers& controllers, const Model& model )
    : controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseFactory destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
ExerciseFactory::~ExerciseFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseFactory::Create
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Exercise* ExerciseFactory::Create( const ASN1T_MsgExerciseCreation& message )
{
    return new Exercise( message, controllers_, model_.terrains_, model_.datasets_, model_.physicalModels_ );
}
