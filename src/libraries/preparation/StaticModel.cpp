// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "StaticModel.h"
#include "TeamKarmas.h"
#include "IntelligenceKarmas.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_gui/DrawingTypes.h"
#include "tools/ExerciseConfig.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( Controllers& controllers )
    : controllers_        ( controllers )
    , coordinateConverter_( *new CoordinateConverter() )
    , detection_          ( *new DetectionMap() )
    , types_              ( *new AgentTypes() )
    , objectTypes_        ( *new ObjectTypes() )
    , levels_             ( *new FormationLevels() )
    , teamKarmas_         ( *new TeamKarmas() )
    , intelligenceKarmas_ ( *new IntelligenceKarmas() )
    , drawings_           ( *new gui::DrawingTypes( controllers.controller_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &drawings_;
    delete &intelligenceKarmas_;
    delete &teamKarmas_;
    delete &levels_;
    delete &objectTypes_;
    delete &types_;
    delete &detection_;
    delete &coordinateConverter_;
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Load( const tools::ExerciseConfig& config )
{
    Purge();
    types_.Load( config );
    objectTypes_.Load( config );
    static_cast< CoordinateConverter& >( coordinateConverter_ ).Load( config );
    detection_.Load( config );
    drawings_.Load( tools::GeneralConfig::BuildResourceChildFile( "DrawingTemplates.xml" ) );
    controllers_.controller_.Update( ModelLoaded( config ) );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    drawings_.Purge();
    types_.Purge();
    objectTypes_.Purge();
    controllers_.controller_.Update( ModelUnLoaded() );
}
    