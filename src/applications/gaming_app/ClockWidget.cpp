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
#include "ClockEditDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Simulation.h"
#include "tools/GeneralConfig.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ClockWidget constructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::ClockWidget( QWidget* parent, kernel::Controllers& controllers, const SimulationController& simulationController )
    : QWidget( parent )
    , controllers_( controllers )
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    setObjectName( "ClockWidget" );
    setBackgroundColor( QColor( 13, 122, 168 ) );
    setAutoFillBackground( true );

    mainLayout->addStretch();

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin( 0 );
    vLayout->setSpacing( 0 );
    vLayout->setSizeConstraint( QLayout::SetMinimumSize );
    const QColor foregroundColor( 173, 230, 255 );
    time_ = new QLabel();
    QFont font = time_->font();
    font.setPixelSize( 40 );
    font.setBold( true );
    time_->setFont( font );
    time_->setPaletteForegroundColor( foregroundColor );
    time_->setText( "00:00:00" );
    time_->setAlignment( Qt::AlignCenter );
    day_ = new QLabel();
    day_->setFixedHeight( 25 );
    day_->setPaletteForegroundColor( foregroundColor );
    day_->setText( tools::translate( "ClockWidget", "Day 1" ) );
    day_->setAlignment( Qt::AlignCenter );

    realDateTime_ = new QLabel();
    realDateTime_->setFixedHeight( 25 );
    realDateTime_->setPaletteForegroundColor( foregroundColor );
    realDateTime_->setAlignment( Qt::AlignCenter );

    QVBoxLayout* timeLayout = new QVBoxLayout();
    timeLayout->setSpacing( 0 );
    timeLayout->addWidget( time_ );
    timeLayout->addWidget( day_ );

    vLayout->addStretch();
    vLayout->addLayout( timeLayout );
    vLayout->addStretch();
    vLayout->addWidget( realDateTime_ );

    mainLayout->addLayout( vLayout );
    mainLayout->addStretch();

    vLayout = new QVBoxLayout();
    vLayout->setMargin( 5 );
    vLayout->setSpacing( 5 );
    vLayout->setAlignment( Qt::AlignTop | Qt::AlignHCenter );

    QPushButton* editButton = new QPushButton();
    editButton->setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/clock_edit.png" ) ) );
    editButton->setFlat( true );
    editButton->setFixedSize( 22, 22 );
    QToolTip::add( editButton, tools::translate( "ClockWidget", "Change date and time" ) );

    alarmButton_ = new QPushButton();
    alarmButton_->setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/clock_alarm.png" ) ) );
    alarmButton_->setFlat( true );
    alarmButton_->setFixedSize( 22, 22 );
    QToolTip::add( alarmButton_, tools::translate( "ClockWidget", "Configure alarms" ) );

    vLayout->addWidget( editButton );
    vLayout->addWidget( alarmButton_ );
    mainLayout->addLayout( vLayout );

    setLayout( mainLayout );

    alarmButton_->setVisible( controllers_.GetCurrentMode() != eModes_Replay );

    ClockEditDialog* editor = new ClockEditDialog( this, controllers, simulationController );
    AlarmsWidget* alarms = new AlarmsWidget( this, controllers, simulationController );

    connect( editButton, SIGNAL( clicked() ), editor, SLOT( show() ) );
    connect( alarmButton_, SIGNAL( clicked() ), alarms, SLOT( show() ) );

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
    day_ ->setText( simulation.GetDateAsString() );
    time_->setText( simulation.GetTimeAsString() );
    if( GetCurrentMode() == eModes_Replay )
        realDateTime_->setText( tools::translate( "ClockWidget", "Real time: %1 - %2" ).arg( simulation.GetRealDateAsString() ).arg( simulation.GetRealTimeAsString() ) );
}

// -----------------------------------------------------------------------------
// Name: ClockWidget::NotifyModeChanged
// Created: JSR 2013-10-31
// -----------------------------------------------------------------------------
void ClockWidget::NotifyModeChanged( E_Modes newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    alarmButton_->setVisible( newMode != eModes_Replay );
    realDateTime_->setVisible( newMode == eModes_Replay );
}
