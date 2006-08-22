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
void Layer_ABC::Paint( const ViewFrustum& )
{
    const float infinity = std::numeric_limits<float>::infinity();
    const geometry::Rectangle2f universe( -infinity, -infinity, infinity, infinity );
    ((MapLayer_ABC*)(this))->Paint( universe );
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
