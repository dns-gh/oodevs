// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineMarker.h"
#include "TimelineView.h"
#include "gaming/ActionsScheduler.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineMarker constructor
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
TimelineMarker::TimelineMarker( TimelineView& view, ActionsScheduler& scheduler, kernel::Controllers& controllers )
    : TimelineItem_ABC( view.canvas() )
    , controllers_( controllers )
    , view_( view )
    , scheduler_( scheduler )
{
    setZ( 1100 );
    show();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker destructor
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
TimelineMarker::~TimelineMarker()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Shift
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::Shift( long shift )
{
    scheduler_.Shift( view_.ConvertToSeconds( shift ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Update
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::Update()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Update
// Created: SBO 2008-04-25
// -----------------------------------------------------------------------------
void TimelineMarker::NotifyUpdated( const Simulation& )
{
    update();
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::draw
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::draw( QPainter& painter )
{
    setX( view_.ConvertToPosition( scheduler_.GetDateTime() ) );
    setY( 0 );
    setSize( 3, view_.contentsHeight() );

    const QPen oldPen( painter.pen() );
    QPen p( QColor( 255, 0, 0 ), 2, QPen::SolidLine );
    painter.setPen( p );
    painter.drawLine( rect().topLeft(), rect().bottomLeft() );
    painter.setPen( oldPen );
}
