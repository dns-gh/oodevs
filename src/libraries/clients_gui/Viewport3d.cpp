// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Viewport3d.h"
#include "graphics/ViewFrustum.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Viewport3d constructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
Viewport3d::Viewport3d( const ViewFrustum& frustum )
    : frustum_( frustum )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Viewport3d destructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
Viewport3d::~Viewport3d()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Viewport3d::IsVisible
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
bool Viewport3d::IsVisible( const geometry::Point2f& point ) const
{
    return frustum_.IsVisible( point );
}

// -----------------------------------------------------------------------------
// Name: Viewport3d::IsVisible
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
bool Viewport3d::IsVisible( const geometry::Rectangle2f& rectangle ) const
{
    return frustum_.IsVisible( rectangle );
}
