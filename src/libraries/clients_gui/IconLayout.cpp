// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IconLayout.h"

// -----------------------------------------------------------------------------
// Name: IconLayout constructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
IconLayout::IconLayout()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: IconLayout destructor
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
IconLayout::~IconLayout()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IconLayout::AddIcon
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void IconLayout::AddIcon( const char** xpm, int x, int y )
{
    iconLocations_[ xpm ] = geometry::Point2f( x, y );
}

// -----------------------------------------------------------------------------
// Name: IconLayout::IconLocation
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
const geometry::Point2f& IconLayout::IconLocation( const char** xpm )
{
    return iconLocations_[ xpm ];
}
