// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ClockWidget.h"
#include "AlarmsWidget.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ClockWidget constructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::ClockWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation )
    : QVBox( parent )
    , controllers_( controllers )
{
    setMinimumSize( 200, 80 );
    setPaletteBackgroundColor( Qt::black );
    QFont font;
    font.setPixelSize( 40 );
    font.setBold( true );
    time_ = new QLabel( this );
    time_->setFont( font );
    time_->setPaletteForegroundColor( Qt::green );
    time_->setText( "00:00:00" );
    time_->setAlignment( Qt::AlignCenter );
    QHBox* box = new QHBox( this );
    box->setFixedHeight( 1 );
    box->layout()->setAlignment( Qt::AlignCenter );
    QLabel* spacer = new QLabel( box );
    spacer->setFixedWidth( 100 );
    spacer->setPaletteBackgroundColor( Qt::green );
    day_ = new QLabel( this );
    day_->setFixedHeight( 25 );
    day_->setPaletteForegroundColor( Qt::green );
    day_->setText( tr( "Day 1" ) );
    day_->setAlignment( Qt::AlignCenter );
    controllers_.Register( *this );

    alarms_ = new AlarmsWidget( this, controllers, simulation );
}

// -----------------------------------------------------------------------------
// Name: ClockWidget destructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::~ClockWidget()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ClockWidget::NotifyUpdated
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
void ClockWidget::NotifyUpdated( const Simulation& simulation )
{
    time_->setText( simulation.GetTimeAsString() );
    day_ ->setText( tr( "Day %1" ).arg( simulation.GetDay() ) );
}

// -----------------------------------------------------------------------------
// Name: ClockWidget::mouseDoubleClickEvent
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
void ClockWidget::mouseDoubleClickEvent( QMouseEvent* )
{
    alarms_->show();
}
