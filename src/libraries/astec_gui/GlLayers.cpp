// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "GlLayers.h"
#include "Layer_ABC.h"
#include "ParametersLayer.h"
#include "TerrainLayer.h"
#include "MetricsLayer.h"
#include "LimitsLayer.h"
#include "ObjectsLayer.h"
#include "PopulationsLayer.h"
#include "AgentsLayer.h"
#include "ColorStrategy.h"
#include "astec_gaming/Model.h"
#include "DefaultLayer.h"
#include "Elevation2dLayer.h"
#include "Elevation3dLayer.h"
#include "astec_gaming/DetectionMap.h"
#include "MeteoLayer.h"
#include "astec_gaming/DetectionMap.h"
#include "AgentKnowledgesLayer.h"
#include "PopulationKnowledgesLayer.h"
#include "ObjectKnowledgesLayer.h"
#include "astec_gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: GlLayers constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::GlLayers( Controllers& controllers, const StaticModel& staticModel, Model& model, GraphicSetup_ABC& setup )
    : controllers_( controllers )
    , static_     ( staticModel )
    , model_      ( model )
    , setup_      ( setup )
    , strategy_( new ColorStrategy( controllers_, *this ) )
{
    parameters_ = new ParametersLayer( *this );
    agents_ = new AgentsLayer( controllers_, *this, *strategy_, *this );
}

// -----------------------------------------------------------------------------
// Name: GlLayers destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::~GlLayers()
{
    delete strategy_;
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
    Register( *new Elevation2dLayer( controllers_.controller_, static_.detection_ ) );
    Register( *new Elevation3dLayer( controllers_.controller_, static_.detection_ ) );
    Register( *new TerrainLayer( controllers_, *this, setup_ ) );
    Register( *new MetricsLayer( controllers_, *this ) );
    Register( *new LimitsLayer( controllers_, *this, *strategy_, *parameters_, model_.limits_ ) );
    Register( *new ObjectsLayer( controllers_, *this, *strategy_, *this ) );
    Register( *new PopulationsLayer( controllers_, *this, *strategy_, *this ) );
    Register( *new AgentKnowledgesLayer( controllers_, *this, *strategy_, *this ) );
    Register( *new PopulationKnowledgesLayer( controllers_, *this, *strategy_, *this ) );
    Register( *new ObjectKnowledgesLayer( controllers_, *this, *strategy_, *this ) );
    Register( *agents_ );
    Register( *new MeteoLayer( controllers_, *this ) );
    Register( *parameters_ );
    
    AddDefaultLayer( *new DefaultLayer( controllers_ ) );
}

