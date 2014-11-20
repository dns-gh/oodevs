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
#include "GLView_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LayersRenderPass constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
LayersRenderPass::LayersRenderPass( const std::string& name, bool clear )
    : name_  ( name )
    , clear_ ( clear )
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
void LayersRenderPass::Render( GLView_ABC& view )
{
    if( clear_ )
        glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glColor3f( 1, 1, 1 );
    view.PaintLayers();
}
