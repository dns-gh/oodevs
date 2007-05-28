// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DebugPoints.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DebugPoints constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
DebugPoints::DebugPoints()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugPoints destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
DebugPoints::~DebugPoints()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugPoints::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void DebugPoints::DoUpdate( const DebugPointsMessage& message )
{
    unsigned long size;
    message >> size;

    points_.clear();
    points_.reserve( size );
    for( unsigned i = 0; i < size; ++i )
    {
        double x, y;
        message >> x >> y;
        points_.push_back( geometry::Point2f( float(x), float(y) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DebugPoints::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void DebugPoints::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "DebugPoints" ) )
    {
        GLfloat color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 6.f );
        for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
            if( viewport.IsVisible( *it ) )
            {
                glColor4f( 0, 0, 0, color[3] * 0.5f );
                glLineWidth( 8.f );
                tools.DrawCross( *it, GL_CROSSSIZE );

                glColor4f( 255, 255, 255, 1.0f );
                glLineWidth( 4.f );
                tools.DrawCross( *it, GL_CROSSSIZE );
            }
        glPopAttrib();
    }
}
