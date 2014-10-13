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

#include "GLView_ABC.h"
#include "IconHandler_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: IconsRenderPass constructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
IconsRenderPass::IconsRenderPass( gui::GLView_ABC& tools )
    : tools_    ( tools )
    , viewport_ ( 0, 0, 600, 600 )
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
void IconsRenderPass::Render( MapWidget_ABC& )
{
    if( !tasks_.empty() )
    {
        glPushAttrib( GL_LINE_BIT );
        glEnable( GL_LINE_SMOOTH );
        for( auto it = tasks_.begin(); it != tasks_.end(); ++it )
            RenderIcon( *it );
        tasks_.clear();
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::RenderIcon
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void IconsRenderPass::RenderIcon( const T_IconTask& task )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );
    glRectf( viewport_.Left() - 50, viewport_.Bottom() - 50, viewport_.Right() + 50, viewport_.Top() + 50 );
    const SymbolIcon& symbol = task.first;
    glPushAttrib( GL_CURRENT_BIT );
    tools_.SetCurrentColor( symbol.color_.red() / 255.f, symbol.color_.green() / 255.f, symbol.color_.blue() / 255.f );
    glPopAttrib();
    const float thickness = viewport_.Width() * 1.5f / iconSide_;
    const geometry::Point2f center( 300, 100 );
    const geometry::Point2f infraSymbolCenter( 220, 310 );
    if( ! symbol.symbol_.empty() )
    {
        if( symbol.symbol_.find( "infrastructures" ) == std::string::npos )
            tools_.DrawApp6Symbol( symbol.symbol_, center, 1.f, thickness, 0 );
        else
            tools_.DrawApp6Symbol( symbol.symbol_, infraSymbolCenter, 0.5f, thickness );
    }
    if( ! symbol.level_.empty() )
        tools_.DrawApp6Symbol( symbol.level_, center, 1.f, thickness );

    glFlush();
    QImage image( iconSide_, iconSide_, 32 );
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
