// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineRuler.h"
#include <qpainter.h>

const unsigned short tickHeight_  = 5;
const unsigned short rulerHeight_ = 15; // $$$$ SBO 2007-07-04: 

// -----------------------------------------------------------------------------
// Name: TimelineRuler constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineRuler::TimelineRuler( QCanvas* canvas, QCanvasView* view )
    : TimelineItem_ABC( canvas, QRect( 200, 0, canvas->width(), rulerHeight_ ) )
    , view_( *view )
    , tickStep_( 5 )
    , pageStep_( 100 )
{
    SetOverlayed( true );
    setZ( 1000 );
    show();
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineRuler::~TimelineRuler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::Update
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineRuler::Update()
{
    const QPoint canvasTopLeft = view_.inverseWorldMatrix().map( QPoint( view_.contentsX(), view_.contentsY() ) );
    setY( canvasTopLeft.y() );
    setSize( canvas()->width(), height() );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineRuler::draw( QPainter& painter )
{
    Update();
    const QPoint canvasTopLeft = view_.inverseWorldMatrix().map( QPoint( view_.contentsX(), view_.contentsY() ) );
    painter.fillRect( 0, y(), width(), height(), Qt::white ); // $$$$ SBO 2007-07-04: clearBackground needed ?
    DrawTimeline( painter );
    painter.drawLine( canvasTopLeft.x() + x(), y(), canvasTopLeft.x() + x(), y() + canvas()->height() );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::DrawTimeline
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineRuler::DrawTimeline( QPainter& painter )
{
    const QFont oldFont = painter.font();

    const QFont newFont( "arial", 7 );
    painter.setFont( newFont );
    painter.drawLine( 0, y() + height(), rect().right(), y() + height() );
    for( int i = x(); i < width(); i += tickStep_ )
        if( i % pageStep_ )
            painter.drawLine( i, y(), i, y() + tickHeight_ - 1 );
        else
        {
            painter.drawLine( i, y(), i, y() + height() - 1 );
            painter.drawText( i + 2, y() + height() - 1, QString::number( i - x() ) );
        }
    painter.setFont( oldFont );
}
