// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MapLayerProxy.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: MapLayerProxy constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
MapLayerProxy::MapLayerProxy( Layer_ABC& layer )
    : layer_( layer )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
MapLayerProxy::~MapLayerProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::Initialize
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void MapLayerProxy::Initialize( const geometry::Rectangle2f& extent )
{
    try
    {
        layer_.Initialize( extent );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void MapLayerProxy::Paint( const ViewFrustum& frustum )
{
    try
    {
        layer_.Paint( frustum );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void MapLayerProxy::Paint( const geometry::Rectangle2f& viewport )
{
    try
    {
        layer_.Paint( viewport );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void MapLayerProxy::Paint( Viewport_ABC& viewport )
{
    try
    {
        layer_.Paint( viewport );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::HandleKeyPress
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
bool MapLayerProxy::HandleKeyPress( QKeyEvent* key )
{
    return layer_.HandleKeyPress( key );
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::HandleMousePress
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
bool MapLayerProxy::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return layer_.HandleMousePress( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::HandleMouseDoubleClick
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
bool MapLayerProxy::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return layer_.HandleMouseDoubleClick( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: MapLayerProxy::HandleMouseMove
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
bool MapLayerProxy::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return layer_.HandleMouseMove( mouse, point );
}
