// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LayersRenderPass.h"
#include "MapWidget_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LayersRenderPass constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
LayersRenderPass::LayersRenderPass( MapWidget_ABC& widget, const std::string& name, bool clear )
    : widget_( widget )
    , name_( name )
    , clear_( clear )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayersRenderPass destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
LayersRenderPass::~LayersRenderPass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayersRenderPass::GetName
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string LayersRenderPass::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: LayersRenderPass::Render
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void LayersRenderPass::Render( MapWidget_ABC& widget )
{
    if( clear_ )
        glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glColor3f( 1, 1, 1 );
    widget.PaintLayers();
}

// -----------------------------------------------------------------------------
// Name: LayersRenderPass::Width
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
unsigned int LayersRenderPass::Width() const
{
    return widget_.Width();
}

// -----------------------------------------------------------------------------
// Name: LayersRenderPass::Height
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
unsigned int LayersRenderPass::Height() const
{
    return widget_.Height();
}

// -----------------------------------------------------------------------------
// Name: LayersRenderPass::Viewport
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
geometry::Rectangle2f LayersRenderPass::Viewport() const
{
    return widget_.Viewport();
}
