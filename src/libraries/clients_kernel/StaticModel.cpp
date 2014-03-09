// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "StaticModel.h"

#include "AccommodationTypes.h"
#include "AgentTypes.h"
#include "AtlasNatures.h"
#include "CoordinateConverter.h"
#include "DetectionMap.h"
#include "DisasterTypes.h"
#include "ExtensionTypes.h"
#include "ObjectTypes.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
StaticModel::StaticModel()
    : coordinateConverter_( *new CoordinateConverter() )
    , types_              ( *new AgentTypes() )
    , disasterTypes_      ( *new DisasterTypes() )
    , objectTypes_        ( *new ObjectTypes() )
    , extensions_         ( *new ExtensionTypes() )
    , atlasNatures_       ( *new AtlasNatures() )
    , accommodationTypes_ ( *new AccommodationTypes() )
    , detection_          ( *new DetectionMap() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &detection_;
    delete &accommodationTypes_;
    delete &atlasNatures_;
    delete &extensions_;
    delete &disasterTypes_;
    delete &objectTypes_;
    delete &types_;
    delete &coordinateConverter_;
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
void StaticModel::Load( const tools::ExerciseConfig& config )
{
    Purge();
    coordinateConverter_.Load( config );
    types_.Load( config );
    disasterTypes_.Load( config );
    objectTypes_.Load( config );
    extensions_.Load( config );
    accommodationTypes_.Load( config );
    detection_.Load( config );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    types_.Purge();
    disasterTypes_.Purge();
    objectTypes_.Purge();
    extensions_.Purge();
    accommodationTypes_.Purge();
}
