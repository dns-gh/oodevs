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
#include "graphics/ElevationLayer.h"
#include "GLTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::Elevation2dLayer( const ElevationMap& elevation )
    : elevation_( elevation )
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
        layer_ = new ElevationLayer( elevation_ );
        layer_->Initialize( extent );
    };
}
    
// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( const geometry::Rectangle2f& viewport )
{
    layer_->Paint( viewport );
}
