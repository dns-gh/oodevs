// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "LocalExerciseFilter.h"
#include "Exercise_ABC.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: LocalExerciseFilter constructor
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
LocalExerciseFilter::LocalExerciseFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalExerciseFilter destructor
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
LocalExerciseFilter::~LocalExerciseFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalExerciseFilter::Allows
// Created: SBO 2010-10-26
// -----------------------------------------------------------------------------
bool LocalExerciseFilter::Allows( const Exercise_ABC& exercise ) const
{
    return exercise.IsHostedBy( "localhost" ) || exercise.IsHostedBy( "127.0.0.1" );
}