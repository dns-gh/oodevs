// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MiniViewsRenderPass.h"
#include "MiniView.h"
#include "MapWidget_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
MiniViewsRenderPass::MiniViewsRenderPass( MiniView& view )
    : view_( view )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
MiniViewsRenderPass::~MiniViewsRenderPass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass::GetName
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string MiniViewsRenderPass::GetName() const
{
    return "miniviews";
}

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass::Render
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void MiniViewsRenderPass::Render( MapWidget_ABC& widget )
{
    widget.PaintLayers();
    QImage image( miniViewSide_, miniViewSide_, 32 );
    glFlush();
    glReadPixels( 0, 0, miniViewSide_, miniViewSide_, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();
    view_.SetImage( image.mirror() );
}

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass::Width
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
unsigned int MiniViewsRenderPass::Width() const
{
    return miniViewSide_;
}

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass::Height
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
unsigned int MiniViewsRenderPass::Height() const
{
    return miniViewSide_;
}

// -----------------------------------------------------------------------------
// Name: MiniViewsRenderPass::Viewport
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
geometry::Rectangle2f MiniViewsRenderPass::Viewport() const
{
    return view_.GetViewport();
}
