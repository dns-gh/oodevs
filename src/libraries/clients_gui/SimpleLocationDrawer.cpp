// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SimpleLocationDrawer.h"

#include "GLView_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer constructor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
SimpleLocationDrawer::SimpleLocationDrawer( const GLView_ABC& tools )
    : tools_( tools )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer constructor
// Created: LGY 2014-07-22
// -----------------------------------------------------------------------------
SimpleLocationDrawer::SimpleLocationDrawer( const GLView_ABC& tools, const QColor& color )
    : tools_( tools )
    , color_( color )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer destructor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
SimpleLocationDrawer::~SimpleLocationDrawer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitLines
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitLines( const T_PointVector& points )
{
    if( !points.empty() )
        tools_.DrawLines( points );
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitRectangle
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitRectangle( const T_PointVector& points )
{
    if( !points.empty() )
        tools_.DrawRectangle( points );
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitPolygon
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitPolygon( const T_PointVector& points )
{
    VisitLines( points );
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitPath( const geometry::Point2f& first, const T_PointVector& points )
{
    if( !points.empty() )
    {
        tools_.DrawLine( first, points.front(), -1.f );
        tools_.DrawLines( points );
    }
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitCircle
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitCircle( const geometry::Point2f& center, float radius )
{
    if( !center.IsZero() && radius > 0 )
    {
        tools_.DrawCross( center );
        tools_.DrawCircle( center, radius );
    }
}

namespace
{
    unsigned int ComputeSegmentNumber( const geometry::Point2f& from, const geometry::Point2f& to )
    {
        unsigned int result =  static_cast< unsigned int >( from.Distance( to ) );
        if( result < 2 )
            result = 2;
        if( result > 20 )
            result = 20;
        return result;
    }
    geometry::Point2f Compute( const geometry::Point2f& from, const geometry::Point2f& control, const geometry::Point2f& to,
        unsigned int i, unsigned int segment )
    {
        float u =  i / static_cast< float >( segment );
        const float nu = 1.f - u;
        const float b0 = 1 * nu * nu;
        const float b1 = 2 *  u * nu;
        const float b3 = 1 *  u *  u;
        return geometry::Point2f( from.X() * b0 + control.X() * b1 + to.X() * b3,
            from.Y() * b0 + control.Y() * b1 + to.Y() * b3 );
    }
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitCurve( const T_PointVector& points )
{
    if( points.size() <= 2 )
        tools_.DrawLines( points );
    else if( points.size() == 3 )
    {
        unsigned int segment = ComputeSegmentNumber( points[ 0 ], points[ 1 ] );
        T_PointVector newPoints;
        for( unsigned int i = 0; i <= segment; ++i )
            newPoints.push_back( Compute( points[ 0 ], points[ 2 ], points[ 1 ], i, segment ) );
        tools_.DrawLines( newPoints );
    }
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitPoint
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitPoint( const geometry::Point2f& point )
{
    if( !point.IsZero() )
        tools_.DrawCross( point, -1.f, GLView_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitText
// Created: LGY 2014-07-21
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitText( const QString& text, const QFont& font, const geometry::Point2f& point )
{
    if( color_.isValid() )
        tools_.DrawShapeText( tools::DrawText( text, font, color_ ), point );
}
