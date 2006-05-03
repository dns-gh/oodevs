// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
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
#include "Model.h"
#include "DefaultLayer.h"
#include "Elevation2dLayer.h"
#include "Elevation3dLayer.h"
#include "DetectionMap.h"
#include "MeteoLayer.h"
#include "DetectionMap.h"

// -----------------------------------------------------------------------------
// Name: GlLayers constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::GlLayers( Controllers& controllers, Model& model, GraphicSetup_ABC& setup )
    : controllers_( controllers )
    , model_      ( model )
    , setup_      ( setup )
    , strategy_( new ColorStrategy( controllers_, *this ) )
{
    parameters_ = new ParametersLayer( *this );
    agents_ = new AgentsLayer( controllers_, *this, *strategy_, *this );
}

// -----------------------------------------------------------------------------
// Name: GlLayers::Load
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
void GlLayers::Load( const std::string& scipioXml )
{
    // $$$$ AGE 2006-05-03: old layers !
    WorldParameters::Load( scipioXml );

    Register( *new Elevation2dLayer( controllers_.controller_, model_.detection_ ) );
    Register( *new Elevation3dLayer( controllers_.controller_, model_.detection_ ) );
    Register( *new TerrainLayer( controllers_, *this, setup_, graphicsDirectory_ ) );
    Register( *new MetricsLayer( controllers_, *this ) );
    Register( *new LimitsLayer( controllers_, *this, *strategy_, *parameters_, model_.limits_ ) );
    Register( *new ObjectsLayer( controllers_, *this, *strategy_, *this ) );
    Register( *new PopulationsLayer( controllers_, *this, *strategy_, *this ) );
    Register( *agents_ );
    Register( *new MeteoLayer( controllers_, *this ) );
    Register( *parameters_ );
    
    AddDefaultLayer( *new DefaultLayer( controllers_ ) );
}

// -----------------------------------------------------------------------------
// Name: GlLayers destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::~GlLayers()
{
    delete strategy_;
}