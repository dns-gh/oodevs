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
#include "moc_TimelineRuler.cpp"
#include "gaming/Simulation.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>

const unsigned int TimelineRuler::tickStep_ = 40;  // 1h => 20px

// -----------------------------------------------------------------------------
// Name: TimelineRuler constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineRuler::TimelineRuler( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation, const unsigned int height )
    : QWidget     ( parent )
    , controllers_( controllers )
    , simulation_ ( simulation )
    , startX_     ( 0 )
    , tickHeight_ ( height / 2 )
{
    setFixedHeight( height );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineRuler::~TimelineRuler()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::NotifyUpdated
// Created: SBO 2008-04-25
// -----------------------------------------------------------------------------
void TimelineRuler::NotifyUpdated( const Simulation& )
{
    repaint();
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::paintEvent
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineRuler::paintEvent( QPaintEvent* )
{
    QPainter painter( this );
    painter.fillRect( 0, 0, width(), height(), Qt::white );
    DrawTimeline( painter );
}

namespace
{
    QDateTime FloorHour( const QDateTime& datetime )
    {
        QDateTime result( datetime.date() );
        result = result.addSecs( datetime.time().hour() * 3600 );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::DrawTimeline
// Created: SBO 2008-04-23
// -----------------------------------------------------------------------------
void TimelineRuler::DrawTimeline( QPainter& painter ) const
{
    const QFont oldFont = painter.font();
    const QFont newFont( "arial", 7 );
    painter.setFont( newFont );
    painter.drawLine( 0, height(), width(), height() );
    painter.drawLine( 0, height() - tickHeight_, width(), height() - tickHeight_ );

    const unsigned int initialOffset = FloorHour( simulation_.GetInitialDateTime() )
                                         .secsTo( simulation_.GetInitialDateTime() ) * tickStep_ / 3600;

    const unsigned int hourOffset = ( startX_ / tickStep_ ) % 24;
    for( unsigned int h = hourOffset; h * tickStep_ < width() + startX_; ++h )
    {
        const unsigned int x = h * tickStep_ - startX_ - initialOffset;
        painter.drawLine( x, height() - tickHeight_, x, height() );
        painter.drawText( x + 2, height() - 1, Time( h ) );
        if( Time( h ).toInt() == 0 ) // $$$$ SBO 2008-04-23: 
        {
            painter.drawLine( x, 0, x, height() );
            painter.drawText( x + 2, height() - tickHeight_ - 1, Day( h / 24 + 1 ) );
        }
    }
    painter.setFont( oldFont );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::SetContentsPos
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineRuler::SetContentsPos( int x, int )
{
    startX_ = x;
    repaint();
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::Day
// Created: SBO 2008-04-23
// -----------------------------------------------------------------------------
QString TimelineRuler::Day( unsigned int dayOffset ) const
{
    const QDateTime day = simulation_.GetInitialDateTime().addDays( dayOffset );
    return day.date().toString( "MMMM d" );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::Time
// Created: SBO 2008-04-23
// -----------------------------------------------------------------------------
QString TimelineRuler::Time( unsigned int hourOffset ) const
{
    const QDateTime hour = simulation_.GetInitialDateTime().addSecs( hourOffset * 3600 );
    return hour.time().toString( "hh" );
}
