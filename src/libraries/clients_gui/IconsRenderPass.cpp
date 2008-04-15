// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IconsRenderPass.h"
#include "GlWidget.h"
#include "IconHandler_ABC.h"
#include <qbitmap.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: IconsRenderPass constructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
IconsRenderPass::IconsRenderPass()
    : viewport_( 0, 0, 600, 600 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass destructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
IconsRenderPass::~IconsRenderPass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::CreateIcon
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void IconsRenderPass::CreateIcon( const SymbolIcon& symbol, IconHandler_ABC& handler )
{
    tasks_.push_back( T_IconTask( symbol, &handler ) );
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::GetName
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string IconsRenderPass::GetName() const
{
    return "icons";
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::Render
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void IconsRenderPass::Render( GlWidget& widget )
{
    glEnable( GL_LINE_SMOOTH );
    for( CIT_IconTasks it = tasks_.begin(); it != tasks_.end(); ++it )
        RenderIcon( *it, widget );
    tasks_.clear();
    glDisable( GL_LINE_SMOOTH );
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::RenderIcon
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void IconsRenderPass::RenderIcon( const T_IconTask& task, GlWidget& widget )
{
    QImage image( iconSide_, iconSide_, 32 );
    glColor3f( 1, 1, 1 );
    glRectf( viewport_.Left() - 50, viewport_.Bottom() - 50, viewport_.Right() + 50, viewport_.Top() + 50 );
    const SymbolIcon& symbol = task.first;
    widget.SetCurrentColor( symbol.color_.red() / 255.f, symbol.color_.green() / 255.f, symbol.color_.blue() / 255.f );
//    windowWidth_ = windowHeight_ = viewport_.Width() * 1.5f; // => trait svg de 2 px
    const float thickness = viewport_.Width() * 1.5f / iconSide_;
    const geometry::Point2f center( 300, 100 );
    widget.DrawApp6Symbol( symbol.symbol_, center, 1.f, thickness );
    if( ! symbol.level_.empty() )
        widget.DrawApp6Symbol( symbol.level_, center, 1.f, thickness );
//    windowWidth_ = windowHeight_ = iconSide_;

    glFlush();
    glReadPixels( 0, 0, iconSide_, iconSide_, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();

    QPixmap result( image.mirror().smoothScale( symbol.size_ ) );
    result.setMask( result.createHeuristicMask( true ) );
    task.second->AddIcon( task.first, result );
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::Width
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
unsigned int IconsRenderPass::Width() const
{
    return iconSide_;
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::Height
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
unsigned int IconsRenderPass::Height() const
{
    return iconSide_;
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::Viewport
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
geometry::Rectangle2f IconsRenderPass::Viewport() const
{
    return viewport_;
}
