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

#include "GlTools_ABC.h"
#include "Viewport_ABC.h"

using namespace gui;

namespace
{
    const geometry::Point2f notSet = geometry::Point2f( -1.f, -1.f );
    const int size = 20;
    const float half = size / 2.f;

    void DrawDisc( QPainter& p, QColor contour, QColor inside, float radius )
    {
        p.setPen( QPen( contour, 2 ) );
        p.setBrush( QBrush( inside ) );
        p.drawEllipse( QPointF( half, half ), radius, radius );
    }
    QImage MakeBitmap( QColor disc, QColor circle )
    {
        QPixmap pm( size, size );
        pm.fill( Qt::transparent );
        QPainter p( &pm );
        p.setRenderHint( QPainter::Antialiasing, true );
        DrawDisc( p, disc, disc, 5 );
        DrawDisc( p, circle, disc, 4 );
        return pm.convertToImage().mirror();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfilerLayer::TerrainProfilerLayer( const GlTools_ABC& tools )
    : tools_  ( tools )
    , from_   ( notSet )
    , to_     ( notSet )
    , current_( notSet )
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
void TerrainProfilerLayer::Paint( Viewport_ABC& viewport )
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
                tools_.DrawSvg( "flag.svg", from_, tools_.GetAdaptiveZoomFactor( false ) );
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
                tools_.DrawSvg( "flag.svg", to_, tools_.GetAdaptiveZoomFactor( false ) );
            }
            if( current_ != notSet && viewport.IsVisible( current_ ) )
            {
                static const QImage normal = MakeBitmap( Qt::yellow, Qt::magenta );
                const float factor = tools_.Pixels( current_ );
                tools_.DrawImage( normal,
                    geometry::Point2f( current_.X() - half * factor, current_.Y() - half * factor ) );
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

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer::SetCurrentPosition
// Created: LGY 2014-06-18
// -----------------------------------------------------------------------------
void TerrainProfilerLayer::SetCurrentPosition( const geometry::Point2f& point )
{
    current_ = point;
}

// -----------------------------------------------------------------------------
// Name: TerrainProfilerLayer::ClearCurrentPosition
// Created: LGY 2014-06-18
// -----------------------------------------------------------------------------
void TerrainProfilerLayer::ClearCurrentPosition()
{
    current_ = notSet;
}
