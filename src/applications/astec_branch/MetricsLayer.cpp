// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MetricsLayer.h"

// -----------------------------------------------------------------------------
// Name: MetricsLayer constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
MetricsLayer::MetricsLayer()
    : gridStep_( 10000 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer destructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
MetricsLayer::~MetricsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::Displace
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
float MetricsLayer::Displace( float value )
{
    if( gridStep_ <= 0 )
        return value;
    int clipped = int( std::floor( value / gridStep_ ) );
    return clipped * gridStep_;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::Paint
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void MetricsLayer::Paint( const geometry::Rectangle2f& viewport )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glColor4d( 1.0, 1.0, 1.0, 0.3 );
    glLineWidth( 1.0 );

    glBegin( GL_LINES );
        for( float x = Displace( viewport.Left() ); x < viewport.Right(); x += gridStep_ )
        {
            glVertex2f( x, viewport.Top()    );
            glVertex2f( x, viewport.Bottom() );
        }

        for( float y = Displace( viewport.Bottom() ); y < viewport.Top(); y += gridStep_ )
        {
            glVertex2f( viewport.Left(),  y );
            glVertex2f( viewport.Right(), y );
        }
    glEnd();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::HandleMousePress
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
bool MetricsLayer::HandleMousePress( Qt::ButtonState button, const geometry::Point2f& point )
{
    // bleh
    return true;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::HandleMouseMove
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
bool MetricsLayer::HandleMouseMove( Qt::ButtonState button, const geometry::Point2f& point )
{
    // bleh
    return true;
}
