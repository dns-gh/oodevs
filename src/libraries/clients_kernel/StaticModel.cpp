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
#include "CoordinateSystems.h"
#include "FormationLevels.h"
#include "ObjectTypes.h"
#include "ExtensionTypes.h"
#include "tools/ExerciseConfig.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
StaticModel::StaticModel()
    : coordinateSystems_  ( *new CoordinateSystems() )
    , coordinateConverter_( *new CoordinateConverter( coordinateSystems_ ) )
    , types_              ( *new AgentTypes() )
    , objectTypes_        ( *new ObjectTypes() )
    , levels_             ( *new FormationLevels() )
    , extensionTypes_     ( *new ExtensionTypes() )
    , atlasNatures_       ( *new AtlasNatures() )
    , accommodationTypes_ ( *new AccommodationTypes() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &atlasNatures_;
    delete &extensionTypes_;
    delete &levels_;
    delete &objectTypes_;
    delete &types_;
    delete &coordinateConverter_;
    delete &coordinateSystems_;
    delete &accommodationTypes_;
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
void StaticModel::Load( const tools::ExerciseConfig& config )
{
    Purge();
    types_.Load( config );
    accommodationTypes_.Load( config );
    objectTypes_.Load( config );
    extensionTypes_.Load( config );
    static_cast< kernel::CoordinateConverter& >( coordinateConverter_ ).Load( config );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    types_.Purge();
    extensionTypes_.Purge();
    objectTypes_.Purge();
}
