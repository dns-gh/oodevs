// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichDateTimeEdit.h"
#include "moc_RichDateTimeEdit.cpp"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichDateTimeEdit::RichDateTimeEdit( const QString& objectName, QWidget* parent /* = 0 */ )
    : RichWarnWidget< QWidget >( objectName, parent )
    , date_( 0 )
    , time_( 0 )
{
    date_ = new RichWarnWidget< QDateEdit >( "date" );
    date_->setCalendarPopup( true );
    date_->setTimeSpec( Qt::UTC );
    time_ = new RichWarnWidget< QTimeEdit >( "time" );
    time_->setTimeSpec( Qt::UTC );
    time_->clearMinimumDateTime();
    time_->clearMaximumDateTime();
    connect( date_, SIGNAL( dateChanged( const QDate& ) ), this, SLOT( OnDateChanged( const QDate& ) ) );
    connect( time_, SIGNAL( timeChanged( const QTime& ) ), this, SLOT( OnTimeChanged( const QTime& ) ) );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 5 );
    layout->addWidget( date_ );
    layout->addWidget( time_ );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit destructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichDateTimeEdit::~RichDateTimeEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::Warn
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::Warn( int msec /*= 3000*/ )
{
    date_->Warn( msec );
    time_->Warn( msec );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::date
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
QDate RichDateTimeEdit::date() const
{
    return date_->date();
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::dateTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
QDateTime RichDateTimeEdit::dateTime() const
{
    return dateTime_;
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::time
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
QTime RichDateTimeEdit::time() const
{
    return time_->time();
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::setMinimumDateTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::setMinimumDateTime( const QDateTime& dateTime )
{
    date_->setMinimumDateTime( dateTime );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::setMaximumDateTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::setMaximumDateTime( const QDateTime& dateTime )
{
    date_->setMaximumDateTime( dateTime );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::setDateTimeRange
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::setDateTimeRange( const QDateTime& min, const QDateTime& max )
{
    date_->setDateTimeRange( min, max );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::setDate
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::setDate( const QDate& date )
{
    dateTime_.setDate( date );
    date_->setDate( date );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::setDateTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::setDateTime( const QDateTime& dateTime )
{
    date_->setDate( dateTime.date() );
    time_->setTime( dateTime.time() );
    dateTime_ = dateTime;
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::setTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::setTime( const QTime& time )
{
    dateTime_.setTime( time );
    time_->setTime( time );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::OnDateChanged
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::OnDateChanged( const QDate& date )
{
    dateTime_.setDate( date );
    emit dateTimeChanged( dateTime_ );
    emit dateChanged( date );
}

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit::OnTimeChanged
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void RichDateTimeEdit::OnTimeChanged( const QTime& time )
{
    dateTime_.setTime( time );
    emit dateTimeChanged( dateTime_ );
    emit timeChanged( time );
}
