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

// -----------------------------------------------------------------------------
// Name: TimelineRuler constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineRuler::TimelineRuler( QWidget* parent, kernel::Controllers& controllers, const unsigned int height )
    : QWidget         ( parent )
    , controllers_    ( controllers )
    , initialDateTime_()
    , tickHeight_     ( unsigned short( height / 2 ) )
    , tickStep_       ( 40 ) // default: 1h => 40px
    , startX_         ( 0 )
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
void TimelineRuler::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.GetInitialDateTime() != initialDateTime_ )
    {
        initialDateTime_ = simulation.GetInitialDateTime();
        update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::paintEvent
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineRuler::paintEvent( QPaintEvent* )
{
    QPainter painter( this );
    painter.fillRect( rect(), Qt::white );
    DrawTimeline( painter );
    painter.drawLine( 0, 0, 0, height() );
    painter.drawLine( 0, 0, width() -1, 0 );
    painter.drawLine( width() - 1, 0, width() - 1, height() );
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
// Name: TimelineRuler::DrawTimeline2
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineRuler::DrawTimeline( QPainter& painter ) const
{
    const QFont oldFont = painter.font();
    const QFont newFont( "arial", 7 );
    painter.setFont( newFont );
    painter.drawLine( 0, height(), width(), height() );
    painter.drawLine( 0, height() - tickHeight_, width(), height() - tickHeight_ );

    DrawDates( painter );
    DrawTimes( painter );

    painter.setFont( oldFont );
}

namespace
{
    QString GetDateText( const QFontMetrics& metrics, const QDate& date, int availableWidth )
    {
        QString result = date.toString();
        if( metrics.width( result ) < availableWidth )
            return result;
        result = date.toString( "MMMM d" );
        if( metrics.width( result ) < availableWidth )
            return result;
        result = date.toString( "MM-dd" );
        if( metrics.width( result ) < availableWidth )
            return result;
        return "";
    }

    QString GetTimeText( const QFontMetrics& metrics, const QTime& time, int availableWidth )
    {
        QString result = time.toString();
        if( metrics.width( result ) < availableWidth )
            return result;
        result = time.toString( "hh:mm" );
        if( metrics.width( result ) < availableWidth )
            return result;
        result = time.toString( "hh" );
        if( metrics.width( result ) < availableWidth )
            return result;
        return "";
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::DrawDates
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineRuler::DrawDates( QPainter& painter ) const
{
    const QFontMetrics metrics( painter.font() );
    QDateTime current = initialDateTime_.addSecs( ConvertToSeconds( startX_ ) );
    QDateTime     next( current.date().addDays( 1 ) );
    long lastX = 0;
    while( lastX < width() )
    {
        const long x = long( ConvertToPosition( next ) ) - startX_;
        const QString text = GetDateText( metrics, current.date(), x - lastX );
        painter.drawLine( x, 0, x, height() );
        if( !text.isEmpty() )
            painter.drawText( lastX + 2, height() - tickHeight_ - 1, text );
        current = next;
        next    = next.addDays( 1 );
        lastX = x;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::DrawTimes
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineRuler::DrawTimes( QPainter& painter ) const
{
    const QFontMetrics metrics( painter.font() );
    QDateTime current = initialDateTime_.addSecs( ConvertToSeconds( startX_ ) );
    QDateTime next    = FloorHour( current ).addSecs( 3600 );
    long lastX = 0;
    while( lastX < width() )
    {
        const long x = long( ConvertToPosition( next ) ) - startX_;
        const QString text = GetTimeText( metrics, current.time(), x - lastX );
        painter.drawLine( x, height() - tickHeight_, x, height() );
        if( !text.isEmpty() )
            painter.drawText( lastX + 2, height() - 1, text );
        current = next;
        next    = next.addSecs( 3600 );
        lastX = x;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::SetContentsPos
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineRuler::SetContentsPos( int x, int )
{
    startX_ = x;
    update();
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::ConvertToPosition
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
unsigned long TimelineRuler::ConvertToPosition( const QDateTime& datetime ) const
{
    const int secs = initialDateTime_.secsTo( datetime );
    return secs * tickStep_ / 3600;
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::ConvertToSeconds
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
long TimelineRuler::ConvertToSeconds( long pixels ) const
{
    return pixels * 3600 / int( tickStep_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::ConvertToPixels
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
long TimelineRuler::ConvertToPixels( long secs ) const
{
    return secs * int( tickStep_ ) / 3600;
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::ZoomIn
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineRuler::ZoomIn()
{
    tickStep_ *= 2;
    update();
}

// -----------------------------------------------------------------------------
// Name: TimelineRuler::ZoomOut
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineRuler::ZoomOut()
{
    const unsigned int newStep = std::max< int >( 2, tickStep_ / 2 );
    if( newStep != tickStep_ )
    {
        tickStep_ = newStep;
        update();
    }
}
