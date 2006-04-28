// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Elevation2dLayer.h"
#include "ColoredElevationLayer.h"
#include "GLTools_ABC.h"
#include "Controller.h"
#include "InitializationMessage.h"
#include "Profiler.h"

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::Elevation2dLayer( Controller& controller, const DetectionMap& elevation )
    : controller_( controller )
    , elevation_( elevation )
    , layer_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::~Elevation2dLayer()
{
    delete layer_;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Initialize
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Initialize( const geometry::Rectangle2f& extent )
{
    if( !layer_ )
    {
        controller_.Update( InitializationMessage( "Génération de la texture 2D..." ) );
        layer_ = new ColoredElevationLayer( elevation_ );
        layer_->Initialize( extent );
    };
}
    
// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( const geometry::Rectangle2f& viewport )
{
    PROFILE();
    layer_->Paint( viewport );
}
