// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Viewport2d.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Viewport2d constructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
Viewport2d::Viewport2d( const geometry::Rectangle2f& viewport )
    : viewport_( viewport )
{
    viewport_.Incorporate( viewport_.BottomLeft() - geometry::Vector2f( 600, 600 ) );
    viewport_.Incorporate( viewport_.TopRight()   + geometry::Vector2f( 600, 600 ) );
}

// -----------------------------------------------------------------------------
// Name: Viewport2d destructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
Viewport2d::~Viewport2d()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Viewport2d::IsVisible
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
bool Viewport2d::IsVisible( const geometry::Point2f& point ) const
{
    return ! viewport_.IsOutside( point );
}

// -----------------------------------------------------------------------------
// Name: Viewport2d::IsVisible
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
bool Viewport2d::IsVisible( const geometry::Rectangle2f& rectangle ) const
{
    return (! viewport_.Intersect( rectangle ).IsEmpty() ) || viewport_.IsInside( viewport_.Intersect( rectangle ).Center() );
}
