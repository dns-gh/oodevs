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

#include "AgentKnowledge.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/ObjectTypes.h"
#include "indicators/GaugeTypes.h"
#include "indicators/Primitives.h"
#include "reports/ReportFactory.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( kernel::Controllers& controllers, const RcEntityResolver_ABC& rcResolver, const kernel::Time_ABC& simulation )
    : kernel::StaticModel()
    , controllers_  ( controllers )
    , indicators_   ( *new indicators::Primitives() )
    , gaugeTypes_   ( *new indicators::GaugeTypes() )
    , drawings_     ( *new gui::DrawingTypes( controllers_.controller_ ) )
    , reportFactory_( *new ReportFactory( rcResolver, objectTypes_, objectTypes_, &simulation ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &reportFactory_;
    delete &drawings_;
    delete &gaugeTypes_;
    delete &indicators_;
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Load( const tools::ExerciseConfig& config )
{
    kernel::StaticModel::Load( config );
    indicators_.Load( *config.GetLoader().LoadFile(
        tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) ));
    gaugeTypes_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ) );
    drawings_.Load( config );
    reportFactory_.Load( config );
    AgentKnowledge::LoadPerceptionAvaibilities( config );
    controllers_.controller_.Update( kernel::ModelLoaded( config ) );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    indicators_.Purge();
    gaugeTypes_.Purge();
    drawings_.Purge();
    reportFactory_.Purge();
    AgentKnowledge::PurgePerceptionAvaibilities();
    kernel::StaticModel::Purge();
    controllers_.controller_.Update( kernel::ModelUnLoaded() );
}
