// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "StaticModel.h"
#include "reports/ReportFactory.h"
#include "SurfaceFactory.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/AtlasNatures.h"
#include "clients_gui/DrawingTypes.h"
#include "indicators/Primitives.h"
#include "indicators/GaugeTypes.h"
#include "tools/ExerciseConfig.h"
#include "urban/StaticModel.h"
#include "urban/Model.h"
#include "UrbanModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( Controllers& controllers, const RcEntityResolver_ABC& rcResolver, const kernel::Time_ABC& simu )
    : controllers_        ( controllers )
    , coordinateSystems_  ( *new CoordinateSystems() )
    , coordinateConverter_( *new CoordinateConverter( coordinateSystems_ ) )
    , detection_          ( *new DetectionMap() )
    , types_              ( *new AgentTypes() )
    , objectTypes_        ( *new ObjectTypes() )
    , magicActionType_    ( *new MagicActionType() )
    , levels_             ( *new FormationLevels() )
    , reportFactory_      ( *new ReportFactory( rcResolver, objectTypes_, objectTypes_, &simu ) )
    , atlasNatures_       ( *new AtlasNatures() )
    , drawings_           ( *new gui::DrawingTypes( controllers_.controller_ ) )
    , indicators_         ( *new indicators::Primitives() )
    , gaugeTypes_         ( *new indicators::GaugeTypes() )
    , urbanTypes_         ( *new urban::StaticModel() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &gaugeTypes_;
    delete &indicators_;
    delete &drawings_;
    delete &atlasNatures_;
    delete &reportFactory_;
    delete &levels_;
    delete &objectTypes_;
    delete &magicActionType_;
    delete &types_;
    delete &detection_;
    delete &coordinateConverter_;
    delete &urbanTypes_;
    delete &coordinateSystems_;
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
    reportFactory_.Load( config );
    drawings_.Load( tools::GeneralConfig::BuildResourceChildFile( "DrawingTemplates.xml" ) );
    indicators_.Load( tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) );
    gaugeTypes_.Load( tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ) );
    controllers_.controller_.Update( ModelLoaded( config ) );
    magicActionType_.Initialize();
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    gaugeTypes_.Purge();
    indicators_.Purge();
    drawings_.Purge();
    reportFactory_.Purge();
    types_.Purge();
    objectTypes_.Purge();
    magicActionType_.Clean();
    //urbanTypes_.Purge();
}
