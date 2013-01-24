// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LocationsLayer.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationsLayer constructor
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
LocationsLayer::LocationsLayer( const gui::GlTools_ABC& tools )
    : tools_( tools )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationsLayer destructor
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
LocationsLayer::~LocationsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationsLayer::Paint
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void LocationsLayer::Paint( Viewport_ABC& viewport )
{
    glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 0, 0, 0, 1 ); // $$$$ SBO 2007-03-28: hard coded, put color in preferences
        for( auto it = locations_.begin(); it != locations_.end(); ++it )
            if( viewport.IsVisible( *it ) )
            {
                tools_.DrawCross( *it );
                tools_.DrawSvg( "flag.svg", *it );
            }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LocationsLayer::AddLocation
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void LocationsLayer::AddLocation( const geometry::Point2f& point )
{
    locations_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: LocationsLayer::Reset
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void LocationsLayer::Reset()
{
    locations_.clear();
}
