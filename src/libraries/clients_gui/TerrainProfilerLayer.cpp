// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainProfilerLayer.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace gui;

namespace
{
    const geometry::Point2f notSet = geometry::Point2f( -1.f, -1.f );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfilerLayer::TerrainProfilerLayer( const kernel::GlTools_ABC& tools )
    : tools_( tools )
    , from_ ( notSet )
    , to_   ( notSet )
{
    SetAlpha( 0 );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer destructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfilerLayer::~TerrainProfilerLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer::Paint
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfilerLayer::Paint( kernel::Viewport_ABC& viewport )
{
    if( !ShouldDrawPass() || GetAlpha() == 0 )
        return;
    if( viewport.IsVisible( geometry::Rectangle2f( from_, to_ ) ) )
    {
        glPushAttrib( GL_CURRENT_BIT );
            if( from_ != notSet && viewport.IsVisible( from_ ) )
            {
                glColor4f( 0, 1, 0, 1 );
                tools_.DrawCross( from_ );
                tools_.DrawSvg( "flag.svg", from_ );
            }
            if( from_ != notSet && to_ != notSet )
            {
                glColor4f( 1, 1, 1, 1 );
                glLineWidth( 3.f );
                tools_.DrawLine( from_, to_ );
                glColor4f( 0, 0, 1, 1 );
                glLineWidth( 1.f );
                tools_.DrawLine( from_, to_ );
            }
            if( to_ != notSet && viewport.IsVisible( to_ ) )
            {
                glColor4f( 0, 0, 1, 1 );
                tools_.DrawCross( to_ );
                tools_.DrawSvg( "flag.svg", to_ );
            }
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer::SetFromPosition
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfilerLayer::SetFromPosition( const geometry::Point2f& point )
{
    from_ = point;
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer::SetToPosition
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfilerLayer::SetToPosition( const geometry::Point2f& point )
{
    to_ = point;
}
