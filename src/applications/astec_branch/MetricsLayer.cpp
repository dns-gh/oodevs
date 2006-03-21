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
#include "GlTools_ABC.h"
#include <iomanip>

// -----------------------------------------------------------------------------
// Name: MetricsLayer constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
MetricsLayer::MetricsLayer( GlTools_ABC& tools )
    : tools_   ( tools )
    , gridStep_( 10000 )
    , ruling_  ( false )
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
// Name: MetricsLayer::Initialize
// Created: AGE 2006-03-20
// -----------------------------------------------------------------------------
void MetricsLayer::Initialize( const geometry::Rectangle2f& extent )
{
    extent_ = extent;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::Paint
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void MetricsLayer::Paint( const geometry::Rectangle2f& v )
{
    geometry::Rectangle2f viewport = v.Intersect( extent_ );
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glColor4d( 1.0, 1.0, 1.0, 0.3 );
    glLineWidth( 1.0 );
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

    if( ruling_ )
    {
        glLineWidth( 2 );
        glColor4d( COLOR_BLACK );
        glBegin( GL_LINES );
            glVertex2fv( (float*)& start_ );
            glVertex2fv( (float*)& end_ );
        glEnd();
        const geometry::Point2f middle( 0.5f*( start_.X() + end_.X() ), 0.5*(start_.Y() + end_.Y() ) );
        std::stringstream message;
        message << "  " << std::setprecision( 2 ) << start_.Distance( end_ ) << "m";
        tools_.Print( message.str(), middle );
    }

    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::HandleMousePress
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
bool MetricsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( ( event->button() & Qt::LeftButton ) && event->state() == Qt::NoButton )
        start_ = point;
    else if( ( event->button() & Qt::LeftButton ) && ( event->state() & Qt::LeftButton ) )
    {
        ruling_ = false;
        start_.Set( 0, 0 );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MetricsLayer::HandleMouseMove
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
bool MetricsLayer::HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point )
{
    if( event->state() == Qt::LeftButton  )
    {
        if( ! start_.IsZero() && start_.Distance( point ) > 10 * tools_.Pixels() )
            ruling_ = true;
        if( ruling_ )
            end_ = point;
    }
    else
    {
        start_.Set( 0, 0 );
        ruling_ = false;
    }
    return ruling_;
}
