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

#include "LogisticLevel.h"
#include "SuccessFactorActionTypes.h"
#include "TeamKarmas.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModelLoaded.h"
#include "indicators/GaugeTypes.h"
#include "indicators/Primitives.h"
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( kernel::Controllers& controllers )
    : kernel::StaticModel()
    , controllers_             ( controllers )
    , indicators_              ( *new indicators::Primitives() )
    , gaugeTypes_              ( *new indicators::GaugeTypes() )
    , drawings_                ( *new gui::DrawingTypes( controllers_.controller_ ) )
    , successFactorActionTypes_( *new SuccessFactorActionTypes() )
    , logisticLevels_          ( *new preparation::LogisticLevel() )
    , teamKarmas_              ( *new TeamKarmas() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &teamKarmas_;
    delete &logisticLevels_;
    delete &successFactorActionTypes_;
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
    indicators_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) );
    gaugeTypes_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ) );
    drawings_.Load( config );
    successFactorActionTypes_.Load( config, tools::GeneralConfig::BuildResourceChildFile( "SuccessFactorActions.xml" ) );
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
    successFactorActionTypes_.Purge();
    kernel::StaticModel::Purge();
    controllers_.controller_.Update( kernel::ModelUnLoaded() );
}
