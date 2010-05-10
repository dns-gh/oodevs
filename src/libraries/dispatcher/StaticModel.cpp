// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "StaticModel.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
StaticModel::StaticModel( const tools::ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    // NOTHING
}
