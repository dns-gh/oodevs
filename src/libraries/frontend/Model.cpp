// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Model.h"
#include "TerrainFactory.h"
#include "DatasetFactory.h"
#include "PhysicalModelFactory.h"
#include "ExerciseFactory.h"
#include "TerrainsModel.h"
#include "DatasetsModel.h"
#include "PhysicalModelsModel.h"
#include "ExercisesModel.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Model::Model( kernel::Controllers& controllers )
    : terrainFactory_( *new TerrainFactory( controllers ) )
    , datasetFactory_( *new DatasetFactory( controllers ) )
    , physicalModelFactory_( *new PhysicalModelFactory( controllers, *this ) )
    , exerciseFactory_( *new ExerciseFactory( controllers, *this ) )
    , terrains_( *new TerrainsModel( terrainFactory_ ) )
    , datasets_( *new DatasetsModel( datasetFactory_ ) )
    , physicalModels_( *new PhysicalModelsModel( physicalModelFactory_ ) )
    , exercises_( *new ExercisesModel( exerciseFactory_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Model::~Model()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Model::Purge()
{
    delete &exercises_;
    delete &physicalModels_;
    delete &datasets_;
    delete &terrains_;
    delete &exerciseFactory_;
    delete &physicalModelFactory_;
    delete &datasetFactory_;
    delete &terrainFactory_;
}
