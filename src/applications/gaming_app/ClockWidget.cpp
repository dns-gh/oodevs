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
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ClockWidget constructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::ClockWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation )
    : QHBox( parent )
    , controllers_( controllers )
{
    setMinimumSize( 200, 80 );
    setPaletteBackgroundColor( Qt::black );
    QVBox* vBox = new QVBox( this );
    QFont font;
    font.setPixelSize( 40 );
    font.setBold( true );
    time_ = new QLabel( vBox );
    time_->setFont( font );
    time_->setPaletteForegroundColor( Qt::green );
    time_->setText( "00:00:00" );
    time_->setAlignment( Qt::AlignCenter );
    QHBox* box = new QHBox( vBox );
    box->setFixedHeight( 1 );
    box->layout()->setAlignment( Qt::AlignCenter );
    QLabel* spacer = new QLabel( box );
    spacer->setFixedWidth( 100 );
    spacer->setPaletteBackgroundColor( Qt::green );
    day_ = new QLabel( vBox );
    day_->setFixedHeight( 25 );
    day_->setPaletteForegroundColor( Qt::green );
    day_->setText( tr( "Day 1" ) );
    day_->setAlignment( Qt::AlignCenter );

    AlarmsWidget* alarms = new AlarmsWidget( this, controllers, simulation );

    vBox = new QVBox( this );
    vBox->setMargin( 5 );
    vBox->setMaximumWidth( 25 );
    vBox->layout()->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
    QPushButton* alarmButton = new QPushButton( vBox );
    alarmButton->setIconSet( MAKE_PIXMAP( timer ) );
    alarmButton->setFlat( true );
    alarmButton->setPaletteBackgroundColor( Qt::black );
    alarmButton->setFixedSize( 22, 22 );
    QToolTip::add( alarmButton, tr( "Configure alarms" ) );
    connect( alarmButton, SIGNAL( clicked() ), alarms, SLOT( show() ) );

    controllers_.Register( *this );

}

// -----------------------------------------------------------------------------
// Name: ClockWidget destructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::~ClockWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ClockWidget::NotifyUpdated
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
void ClockWidget::NotifyUpdated( const Simulation& simulation )
{
    time_->setText( simulation.GetTimeAsString() );
    day_ ->setText( simulation.GetDateAsString() );
}
