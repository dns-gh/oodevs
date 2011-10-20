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
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "tools/GeneralConfig.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ClockWidget constructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::ClockWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation, ActionsScheduler& scheduler )
    : Q3HBox( parent, "ClockWidget" )
    , controllers_( controllers )
{
    static const QColor foregroundColor( 173, 230, 255 );
    static const QColor backgroundColor( 13, 122, 168 );

    setMinimumSize( 200, 80 );
    setStyleSheet("Q3HBox { background-color: #0d7aa8; }");
    {
        Q3VBox* vBox = new Q3VBox( this );
        vBox->setMargin( 5 );
        vBox->setMaximumWidth( 25 );
        vBox->layout()->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
    }
    {
        Q3VBox* vBox = new Q3VBox( this );
        QFont font;
        font.setPixelSize( 40 );
        font.setBold( true );
        time_ = new QLabel( vBox );
        time_->setFont( font );
        //QColor( 142, 219, 251 )
        time_->setPaletteForegroundColor( foregroundColor );
        time_->setText( "00:00:00" );
        time_->setAlignment( Qt::AlignCenter );
        Q3HBox* box = new Q3HBox( vBox );
        box->setFixedHeight( 1 );
        box->layout()->setAlignment( Qt::AlignCenter );
        QLabel* spacer = new QLabel( box );
        spacer->setFixedWidth( 100 );
        spacer->setPaletteBackgroundColor( foregroundColor );
        day_ = new QLabel( vBox );
        day_->setFixedHeight( 25 );
        day_->setPaletteForegroundColor( foregroundColor );
        day_->setText( tools::translate( "ClockWidget", "Day 1" ) );
        day_->setAlignment( Qt::AlignCenter );
    }
    {
        ClockEditDialog* editor = new ClockEditDialog( this, controllers, scheduler );
        AlarmsWidget* alarms = new AlarmsWidget( this, controllers, simulation );
        Q3VBox* vBox = new Q3VBox( this );
        vBox->setMargin( 5 );
        vBox->setMaximumWidth( 25 );
        vBox->layout()->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
        {
            QPushButton* editButton = new QPushButton( vBox );
            editButton->setIconSet( QPixmap( QImage( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/clock_edit.png" ).c_str() ) ) );
            editButton->setFlat( true );
            editButton->setFixedSize( 22, 22 );
            editButton->setPaletteBackgroundColor( backgroundColor );
            QToolTip::add( editButton, tools::translate( "ClockWidget", "Change date and time" ) );
            connect( editButton, SIGNAL( clicked() ), editor, SLOT( show() ) );
        }
        {
            QPushButton* alarmButton = new QPushButton( vBox );
            alarmButton->setIconSet( QPixmap( QImage( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/clock_alarm.png" ).c_str() ) ) );
            alarmButton->setFlat( true );
            alarmButton->setFixedSize( 22, 22 );
            alarmButton->setPaletteBackgroundColor( backgroundColor );
            QToolTip::add( alarmButton, tools::translate( "ClockWidget", "Configure alarms" ) );
            connect( alarmButton, SIGNAL( clicked() ), alarms, SLOT( show() ) );
        }
    }
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
// Name: ClockWidget paintEvent
// Created: SBO 2011-06-22
// -----------------------------------------------------------------------------
void ClockWidget::paintEvent( QPaintEvent* = 0)
{
    QPainter p;
    if( p.begin( this ) )
    {
        p.end();
    }
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
