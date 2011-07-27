// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimeTableRequestDialog.h"
#include "moc_TimeTableRequestDialog.cpp"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/ReplaySenders.h"

// -----------------------------------------------------------------------------
// Name: TimeTableRequestDialog constructor
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
TimeTableRequestDialog::TimeTableRequestDialog( QWidget* parent, Publisher_ABC& network, unsigned int maxTick )
    : QDialog ( parent, 0, true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title )
    , network_( network )    
{
    setCaption( tr( "Time Table Request" ) );

    QGridLayout* pMainLayout = new QGridLayout( this, 3, 2 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Time range" ), this );
    pMainLayout->addMultiCellWidget( box, 0, 1, 0, 2 );

    new QLabel( tr( "Begin tick:" ), box );
    beginTick_ = new QSpinBox( 1, maxTick, 1, box );
    beginTick_->setValue( 1 );

    new QLabel( tr( "End tick:" ), box );
    endTick_ = new QSpinBox( 1, maxTick, 1, box );
    endTick_->setValue( maxTick );

    QPushButton* pOKButton = new QPushButton( tr( "Ok" ), this );
    QPushButton* pCancelButton = new QPushButton( tr( "Cancel" ), this );
    pMainLayout->addWidget( pOKButton, 3, 0 );
    pMainLayout->addWidget( pCancelButton, 3, 1, Qt::AlignRight );
    pOKButton->setDefault( true );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
}

// -----------------------------------------------------------------------------
// Name: TimeTableRequestDialog destructor
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
TimeTableRequestDialog::~TimeTableRequestDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeTableRequestDialog::Validate
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void TimeTableRequestDialog::Validate()
{
    accept();
    replay::TimeTableRequest request;
    request().mutable_tick_range()->set_begin_tick( beginTick_->value() );
    request().mutable_tick_range()->set_end_tick( endTick_->value() );
    request.Send( network_ );
}
