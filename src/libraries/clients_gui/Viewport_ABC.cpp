// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Viewport_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Viewport_ABC constructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
Viewport_ABC::Viewport_ABC()
    : hotIsVisible_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Viewport_ABC destructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
Viewport_ABC::~Viewport_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Viewport_ABC::IsHotpointVisible
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
bool Viewport_ABC::IsHotpointVisible() const
{
    return hotIsVisible_;
}

// -----------------------------------------------------------------------------
// Name: Viewport_ABC::SetHotpoint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Viewport_ABC::SetHotpoint( const geometry::Point2f& point )
{
    hotIsVisible_ = IsVisible( point );
}

void Viewport_ABC::SetHotpoint( const geometry::Rectangle2f& rectangle )
{
    hotIsVisible_ = IsVisible( rectangle );
}
