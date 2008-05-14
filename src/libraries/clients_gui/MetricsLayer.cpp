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
#include "clients_kernel/GlTooltip_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Styles.h"
#include "Tools.h"
#include <qfont.h>

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
void MetricsLayer::Paint( kernel::Viewport_ABC& )
{
    if( ruling_ )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glLineWidth( 4 );
        glColor4f( COLOR_WHITE );
        tools_.DrawLine( start_, end_ );
        glLineWidth( 2 );
        glColor4f( COLOR_BLACK );
        tools_.DrawLine( start_, end_ );
        const geometry::Point2f middle( 0.5f * ( start_.X() + end_.X() ), 0.5f * ( start_.Y() + end_.Y() ) );
        const QString message = tools::translate( "Règle GL", " %1m" ).arg( start_.Distance( end_ ), 0, 'f', 1 );
        if( !tooltip_.get() )
        {
            std::auto_ptr< kernel::GlTooltip_ABC > tooltip( tools_.CreateTooltip() );
            tooltip_ = tooltip;
        }
        // $$$$ SBO 2008-03-19: GlTooltip_ABC maybe should be a Displayer_ABC...
        static_cast< kernel::Displayer_ABC& >( *tooltip_ ).Start( Styles::bold ).Add( message.ascii() ).End();
        tooltip_->Draw( middle );
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
        if( ! start_.IsZero() && start_.Distance( point ) > 10 * tools_.Pixels( point ) )
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
