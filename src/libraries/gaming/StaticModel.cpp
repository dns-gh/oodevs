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
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_gui/DrawingTypes.h"
#include "indicators/Primitives.h"
#include "indicators/GaugeTypes.h"
#include "tools/ExerciseConfig.h"
#include "UrbanModel.h"

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( kernel::Controllers& controllers, const RcEntityResolver_ABC& rcResolver, const kernel::Time_ABC& simulation )
    : controllers_  ( controllers )
    , detection_    ( *new kernel::DetectionMap() )
    , reportFactory_( *new ReportFactory( rcResolver, objectTypes_, objectTypes_, &simulation ) )
    , drawings_     ( *new gui::DrawingTypes( controllers_.controller_ ) )
    , indicators_   ( *new indicators::Primitives() )
    , gaugeTypes_   ( *new indicators::GaugeTypes() )
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
    delete &reportFactory_;
    delete &detection_;
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Load( const tools::ExerciseConfig& config )
{
    kernel::StaticModel::Load( config );
    detection_.Load( config );
    reportFactory_.Load( config );
    drawings_.Load( config, config.BuildPhysicalChildFile( "DrawingTemplates.xml" ) );
    indicators_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) );
    gaugeTypes_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ) );
    controllers_.controller_.Update( kernel::ModelLoaded( config ) );
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
    kernel::StaticModel::Purge();
    controllers_.controller_.Update( kernel::ModelUnLoaded() );
}
