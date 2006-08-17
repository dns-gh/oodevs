// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_prepa_app_pch.h"
#include "GlLayers.h"

#include "astec_gui/ParametersLayer.h"
//#include "astec_gui/AgentsLayer.h"
//#include "astec_gui/ColorStrategy.h"
#include "astec_gui/DefaultLayer.h"
#include "astec_gui/Elevation2dLayer.h"
#include "astec_gui/Elevation3dLayer.h"
#include "astec_gui/LimitsLayer.h"
#include "astec_gui/MetricsLayer.h"
#include "astec_gui/ObjectsLayer.h"
//#include "astec_gui/PopulationsLayer.h"
//#include "astec_gui/TerrainLayer.h"
#include "astec_kernel/DetectionMap.h"
#include "astec_kernel/DetectionMap.h"
#include "astec_preparation/Model.h"
#include "astec_preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: GlLayers constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::GlLayers( Controllers& controllers, const StaticModel& staticModel, Model& model, GraphicSetup_ABC& setup )
    : controllers_( controllers )
    , static_     ( staticModel )
    , model_      ( model )
    , setup_      ( setup )
//    , strategy_( new ColorStrategy( controllers_, *this ) )
{
    parameters_ = new ParametersLayer( *this );
//    agents_ = new AgentsLayer( controllers_, *this, *strategy_, *this );
}

// -----------------------------------------------------------------------------
// Name: GlLayers destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::~GlLayers()
{
//    delete strategy_;
}

// -----------------------------------------------------------------------------
// Name: GlLayers::RegisterBaseLayers
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void GlLayers::RegisterBaseLayers()
{
    // $$$$ AGE 2006-05-17: L'ordre défini à la fois l'ordre de dessin et celui des signaux,
    // $$$$ AGE 2006-05-17: qui ne sont clairement pas compatibles. 
    // $$$$ AGE 2006-05-17: Trouver un truc pas dégueu qui fonctionne. 
    // $$$$ AGE 2006-05-17: Par exemple, filer une stratégie qui dispatcherait les bidules.
//    Register( *new Elevation2dLayer( controllers_.controller_, static_.detection_ ) );
//    Register( *new TerrainLayer( controllers_, *this, setup_ ) );
    Register( *new MetricsLayer( controllers_, *this ) );
//    Register( *new LimitsLayer( controllers_, *this, *strategy_, *parameters_, model_.limits_ ) );
//    Register( *new ObjectsLayer( controllers_, *this, *strategy_, *this ) );
//    Register( *new PopulationsLayer( controllers_, *this, *strategy_, *this ) );
//    Register( *agents_ );
//    Register( *new MeteoLayer( controllers_, *this ) );
    Register( *parameters_ );
    
    AddDefaultLayer( *new DefaultLayer( controllers_ ) );
}

