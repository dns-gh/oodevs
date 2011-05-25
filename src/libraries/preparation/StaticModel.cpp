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
#include "IntelligenceKarmas.h"
#include "SuccessFactorActionTypes.h"
#include "TeamKarmas.h"
#include "LogisticLevel.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/ObjectTypes.h"
#include "indicators/GaugeTypes.h"
#include "indicators/Primitives.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem/path.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( Controllers& controllers )
    : controllers_             ( controllers )
    , coordinateSystems_       ( *new CoordinateSystems() )
    , coordinateConverter_     ( *new CoordinateConverter( coordinateSystems_ ) )
    , detection_               ( *new DetectionMap() )
    , types_                   ( *new AgentTypes() )
    , objectTypes_             ( *new ObjectTypes() )
    , levels_                  ( *new FormationLevels() )
    , accommodationTypes_      ( *new AccommodationTypes() )
    , extensions_              ( *new ExtensionTypes() )
    , teamKarmas_              ( *new TeamKarmas() )
    , intelligenceKarmas_      ( *new IntelligenceKarmas() )
    , drawings_                ( *new gui::DrawingTypes( controllers.controller_ ) )
    , indicators_              ( *new indicators::Primitives() )
    , gaugeTypes_              ( *new indicators::GaugeTypes() )
    , successFactorActionTypes_( *new SuccessFactorActionTypes() )
    , logisticLevels_          ( *new preparation::LogisticLevel() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &successFactorActionTypes_;
    delete &gaugeTypes_;
    delete &indicators_;
    delete &drawings_;
    delete &intelligenceKarmas_;
    delete &teamKarmas_;
    delete &extensions_;
    delete &levels_;
    delete &accommodationTypes_;
    delete &objectTypes_;
    delete &types_;
    delete &detection_;
    delete &coordinateConverter_;
    delete &coordinateSystems_;
    delete &logisticLevels_;
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
    extensions_.Load( config );
    drawings_.Load( config, config.BuildPhysicalChildFile( "DrawingTemplates.xml" ) );
    indicators_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) );
    gaugeTypes_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ) );
    successFactorActionTypes_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "SuccessFactorActions.xml" ) );
    controllers_.controller_.Update( ModelLoaded( config ) );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    successFactorActionTypes_.Purge();
    gaugeTypes_.Purge();
    indicators_.Purge();
    drawings_.Purge();
    types_.Purge();
    objectTypes_.Purge();
    extensions_.Purge();
    controllers_.controller_.Update( ModelUnLoaded() );
}
