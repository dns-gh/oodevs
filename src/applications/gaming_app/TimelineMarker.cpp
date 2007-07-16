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
#include "gaming/ActionsScheduler.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineMarker constructor
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
TimelineMarker::TimelineMarker( QCanvasView* view, ActionsScheduler& scheduler )
    : TimelineItem_ABC( view->canvas() )
    , view_( *view )
    , scheduler_( scheduler )
{
    setZ( 1100 );
    show();
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker destructor
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
TimelineMarker::~TimelineMarker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Shift
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::Shift( long shift )
{
    scheduler_.Shift( shift );
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Update
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::Update()
{
    setX( 200 + scheduler_.GetCurrentTime() );
    setY( 0 );
    setSize( 3, view_.canvas()->height() );
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::draw
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::draw( QPainter& painter )
{
    Update();
    const QPen oldPen( painter.pen() );
    QPen p( QColor( 255, 0, 0 ), 2, QPen::SolidLine );
    painter.setPen( p );
    if( x() < view_.contentsX() + 200 )
        painter.drawLine( rect().left(), rect().top(), rect().left(), view_.contentsY() + 15 );
    else
        painter.drawLine( rect().topLeft(), rect().bottomLeft() );
    painter.setPen( oldPen );
}
