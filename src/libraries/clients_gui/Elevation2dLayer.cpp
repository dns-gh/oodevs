// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Elevation2dLayer.h"
#include "ColoredElevationLayer.h"
#include "clients_kernel/GLTools_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::Elevation2dLayer( Controller& controller, const DetectionMap& elevation )
    : controller_( controller )
    , elevation_( elevation )
    , layer_( 0 )
    , modelLoaded_( false )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::~Elevation2dLayer()
{
    controller_.Remove( *this );
    delete layer_;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void Elevation2dLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    delete layer_; layer_ = 0;
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( GetAlpha() == 0 )
        return;
    if( !layer_ && modelLoaded_ )
    {
        layer_ = new ColoredElevationLayer( elevation_ );
        layer_->Initialize( geometry::Rectangle2f() );
    }
    if( layer_ )
    {
        glColor4f( 1, 1, 1, GetAlpha() );
        layer_->Paint( viewport );
    }
}
