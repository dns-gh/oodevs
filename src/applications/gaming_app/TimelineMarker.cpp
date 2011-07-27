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
#include "TimelineRuler.h"
#include "gaming/ActionsScheduler.h"
#include "gaming/Services.h"
#include "clients_kernel/Controllers.h"
#include "protocol/ReplaySenders.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineMarker constructor
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
TimelineMarker::TimelineMarker( QCanvas* canvas, ActionsScheduler& scheduler, kernel::Controllers& controllers, const TimelineRuler& ruler )
    : TimelineItem_ABC( canvas )
    , controllers_    ( controllers )
    , ruler_          ( ruler )
    , scheduler_      ( scheduler )
    , shift_          ( 0 )
{
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
// Name: TimelineMarker::Move
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::Move( long offset )
{
    if( enabled() )
        shift_ += offset;
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::CommitMove
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineMarker::CommitMove()
{
    if( shift_ )
    {
        scheduler_.Shift( ruler_.ConvertToSeconds( shift_ ) );
        shift_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Update
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void TimelineMarker::Update()
{
    setX( ruler_.ConvertToPosition( scheduler_.GetDateTime() ) );
    setY( 0 );
    setZ( x() + 1 );
    setSize( 3, canvas()->height() );
    update();
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::Update
// Created: SBO 2008-04-25
// -----------------------------------------------------------------------------
void TimelineMarker::NotifyUpdated( const Simulation& )
{
    Update();
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
    painter.drawLine( rect().topLeft(), rect().bottomLeft() );
    painter.setPen( oldPen );
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::DisplayToolTip
// Created: SBO 2008-04-29
// -----------------------------------------------------------------------------
void TimelineMarker::DisplayToolTip( QWidget* parent ) const
{
    const QString tip = scheduler_.GetDateTime().toString();
    if( QToolTip::textFor( parent ) != tip )
    {
        QToolTip::remove( parent );
        QToolTip::add( parent, tip );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineMarker::NotifyUpdated
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
void TimelineMarker::NotifyUpdated( const Services& services )
{
    setEnabled( !services.HasService< replay::Service >() );
}
