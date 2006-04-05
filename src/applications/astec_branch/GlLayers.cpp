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
#include "graphics/ElevationMap.h"
#include "MeteoLayer.h"
#include "DetectionMap.h"

// -----------------------------------------------------------------------------
// Name: GlLayers constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::GlLayers( const std::string& scipioXml, Controllers& controllers, Model& model, GraphicSetup_ABC& setup )
    : WorldParameters( scipioXml )
    , GlProxy( controllers )
    , strategy_( new ColorStrategy( controllers, *this ) )
{
    parameters_ = new ParametersLayer( *this );

    Register( *new Elevation2dLayer( model.detection_ ) );
    Register( *new Elevation3dLayer( model.detection_ ) );
    Register( *new TerrainLayer( controllers, *this, setup, graphicsDirectory_ ) );
    Register( *new MetricsLayer( controllers, *this ) );
    Register( *new LimitsLayer( controllers, *this, *strategy_, *parameters_, model.limits_ ) );
    Register( *new ObjectsLayer( controllers, *this, *strategy_, *this ) );
    Register( *new PopulationsLayer( controllers, *this, *strategy_, *this ) );
    Register( *new AgentsLayer( controllers, *this, *strategy_, *this ) );
    Register( *new MeteoLayer( controllers, *this ) );
    Register( *parameters_ );
    
    AddDefaultLayer( *new DefaultLayer( controllers ) );
}

// -----------------------------------------------------------------------------
// Name: GlLayers destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlLayers::~GlLayers()
{
    delete strategy_;
}