// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RasterLayer.h"
#include "graphics/Visitor2d.h"
#include "graphics/TextureSet.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RasterLayer constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterLayer::RasterLayer( TextureSet& raster )
    : raster_( raster )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RasterLayer destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterLayer::~RasterLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::Paint
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterLayer::Paint( const geometry::Rectangle2f& viewport )
{
    Visitor2d visitor;
    glColor3f( 1, 1, 1 );
    raster_.Accept( visitor, 0, viewport );
}
