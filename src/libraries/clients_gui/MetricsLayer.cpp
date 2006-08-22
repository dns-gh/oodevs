// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MetricsLayer.h"
#include "clients_kernel/GlTools_ABC.h"
#include <iomanip>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MetricsLayer constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
MetricsLayer::MetricsLayer( GlTools_ABC& tools )
    : ruling_( false )
    , tools_ ( tools )
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
// Name: MetricsLayer::Paint
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void MetricsLayer::Paint( const geometry::Rectangle2f& )
{
    if( ruling_ )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glLineWidth( 2 );
        glColor4d( COLOR_BLACK );
        tools_.DrawLine( start_, end_ );
        const geometry::Point2f middle( 0.5f*( start_.X() + end_.X() ), 0.5*(start_.Y() + end_.Y() ) );
        std::stringstream message;
        message << "  " << std::setw( 1 ) << start_.Distance( end_ ) << "m";
        tools_.Print( message.str(), middle );
        glPopAttrib();
    }
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
