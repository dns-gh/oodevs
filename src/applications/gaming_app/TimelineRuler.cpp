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
const unsigned short lineHeight_  = 30; // $$$$ SBO 2007-07-04: 

// -----------------------------------------------------------------------------
// Name: TimelineRuler constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineRuler::TimelineRuler( QCanvas* canvas, QCanvasView* view )
    : QCanvasRectangle( canvas )
    , view_( *view )
    , tickStep_( 5 )
    , pageStep_( 100 )
{
    setX( 200 );
    setY( 0 );
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
// Name: TimelineRuler::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineRuler::draw( QPainter& painter )
{
    const QPoint canvasTopLeft = view_.inverseWorldMatrix().map( QPoint( view_.contentsX(), view_.contentsY() ) );

    setY( canvasTopLeft.y() );
    setSize( canvas()->width(), canvas()->height() );
    painter.fillRect( 0, y(), width(), rulerHeight_, Qt::white ); // $$$$ SBO 2007-07-04: 
    
    DrawGrid( painter );
    DrawTimeline( painter );
    painter.drawLine( canvasTopLeft.x() + x(), y(), canvasTopLeft.x() + x(), y() + height() );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::DrawTimeline
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineRuler::DrawTimeline( QPainter& painter )
{
    const QFont oldFont = painter.font();

    QFont newFont( "arial", 7 );
    painter.setFont( newFont );
    painter.drawLine( 0, y() + rulerHeight_, rect().right(), y() + rulerHeight_ );
    for( int i = x(); i < width(); i += tickStep_ )
        if( i % pageStep_ )
            painter.drawLine( i, y(), i, y() + tickHeight_ - 1 );
        else
        {
            painter.drawLine( i, y(), i, y() + rulerHeight_ - 1 );
            painter.drawText( i + 2, y() + rulerHeight_ - 1, QString::number( i - x() ) );
        }
    painter.setFont( oldFont );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::DrawGrid
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineRuler::DrawGrid( QPainter& painter )
{
    const QPen oldPen = painter.pen();
    QPen p;
    p.setStyle( QPen::DotLine );
    p.setColor( QColor( 150, 150, 150 ) );
    painter.setPen( p );
    for( int i = rulerHeight_; i < height(); i += lineHeight_ )
        if( i > y() + rulerHeight_ )
            painter.drawLine( 0, i, width(), i );
    painter.setPen( oldPen );
}
