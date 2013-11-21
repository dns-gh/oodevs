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

#include "GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer constructor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
SimpleLocationDrawer::SimpleLocationDrawer( const GlTools_ABC& tools )
    : tools_( tools )
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

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitCurve( const T_PointVector& /*points*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleLocationDrawer::VisitPoint
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SimpleLocationDrawer::VisitPoint( const geometry::Point2f& point )
{
    if( !point.IsZero() )
        tools_.DrawCross( point, -1.f, GlTools_ABC::pixels );
}
