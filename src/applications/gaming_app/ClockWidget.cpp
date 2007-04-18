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
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ClockWidget constructor
// Created: SBO 2007-04-17
// -----------------------------------------------------------------------------
ClockWidget::ClockWidget( QWidget* parent, kernel::Controllers& controllers )
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
    unsigned long seconds = simulation.IsConnected() ? simulation.GetTime() : 0;
    time_->setText( QString( "%1:%2:%3" ).arg( QString::number( ( seconds / 3600 ) % 24 ).rightJustify( 2, '0' ) )
                                         .arg( QString::number( ( seconds / 60   ) % 60 ).rightJustify( 2, '0' ) )
                                         .arg( QString::number( ( seconds % 60   )      ).rightJustify( 2, '0' ) ) );
    day_->setText( tr( "Day %1" ).arg( 1 + seconds / ( 3600 * 24 ) ) );
}
