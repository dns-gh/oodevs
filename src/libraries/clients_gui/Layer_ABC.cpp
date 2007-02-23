// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Layer_ABC.h"
#include "Gl3dWidget.h"
#include "GlWidget.h"
#include "MapLayerProxy.h"
#include "Viewport2d.h"
#include "Viewport3d.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Layer_ABC constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer_ABC::Layer_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer_ABC::~Layer_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer_ABC::Paint( const ViewFrustum& frustum )
{
    Viewport3d viewport( frustum );
    Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer_ABC::Paint( const geometry::Rectangle2f& rectangle )
{
    Viewport2d viewport( rectangle );
    Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer_ABC::Paint( kernel::Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::RegisterIn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer_ABC::RegisterIn( Gl3dWidget& widget )
{
    widget.Register( *new MapLayerProxy( *this ) );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::RegisterIn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer_ABC::RegisterIn( GlWidget& widget )
{
    widget.Register( *new MapLayerProxy( *this ) );
}   

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Reset2d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Layer_ABC::Reset2d()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Layer_ABC::Reset3d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Layer_ABC::Reset3d()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer2d_ABC::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2d_ABC::SetAlpha( float alpha )
{
    alpha_ = alpha;
}

// -----------------------------------------------------------------------------
// Name: Layer2d_ABC::GetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
float Layer2d_ABC::GetAlpha() const
{
    return alpha_;
}

// -----------------------------------------------------------------------------
// Name: Layer2d_ABC::Reset2d
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2d_ABC::Reset2d()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Layer2d_ABC::Reset
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2d_ABC::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer3d_ABC::Reset3d
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer3d_ABC::Reset3d()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Layer3d_ABC::Reset
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer3d_ABC::Reset()
{
    // NOTHING
}
