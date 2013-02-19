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
#include "IconHandler_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: IconsRenderPass constructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
IconsRenderPass::IconsRenderPass( kernel::GlTools_ABC& tools )
    : tools_    ( tools )
    , viewport_ ( 0, 0, 600, 600 * RENDER_FACTOR )
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
        glEnable( GL_LINE_SMOOTH );
        for( auto it = tasks_.begin(); it != tasks_.end(); ++it )
            RenderIcon( *it );
        tasks_.clear();
        glDisable( GL_LINE_SMOOTH );
    }
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::RenderIcon
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void IconsRenderPass::RenderIcon( const T_IconTask& task )
{
    const SymbolIcon& symbol = task.first;
    glClear( GL_DEPTH_BUFFER_BIT );
    glClearColor( symbol.color_.red() / 255.f, symbol.color_.green() / 255.f, symbol.color_.blue() / 255.f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );
    tools_.SetCurrentColor( symbol.color_.red() / 255.f, symbol.color_.green() / 255.f, symbol.color_.blue() / 255.f );
    const float thickness = viewport_.Width() * 1.5f / iconSide_;
    const geometry::Point2f app6SymbolCenter( 300, 150 );
    const geometry::Point2f infraSymbolCenter( 220, 510 );
    const geometry::Point2f levelCenter( 300, symbol.karmaFactor_ );
    if( ! symbol.symbol_.empty() )
    {
        if( symbol.symbol_.find( "infrastructures" ) == std::string::npos )
            tools_.DrawApp6Symbol( symbol.symbol_, app6SymbolCenter, 1.f, thickness );
        else
            tools_.DrawApp6Symbol( symbol.symbol_, infraSymbolCenter, 0.5f, thickness );
    }
    if( ! symbol.level_.empty() )
        tools_.DrawApp6Symbol( symbol.level_, levelCenter, 1.5f, 3.f );

    glFlush();
    QImage image( iconSide_, static_cast< unsigned int >( iconSide_ * RENDER_FACTOR ), QImage::Format_ARGB32 );
    glReadPixels( 0, 0, iconSide_, static_cast< unsigned int >( iconSide_ * RENDER_FACTOR ), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();

    QPixmap result( image.mirrored() );
    result.setMask( result.createHeuristicMask( true ) );

    glClearColor( 0.f, 0.f, 0.f, 0.f );

    QSize size( symbol.size_.width(), static_cast< unsigned int >( symbol.size_.height() * RENDER_FACTOR ) );
    task.second->AddIcon( task.first, QPixmap( result.toImage().scaled( size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation ) ) );
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
    return static_cast< unsigned int >( iconSide_ * RENDER_FACTOR );
}

// -----------------------------------------------------------------------------
// Name: IconsRenderPass::Viewport
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
geometry::Rectangle2f IconsRenderPass::Viewport() const
{
    return viewport_;
}
