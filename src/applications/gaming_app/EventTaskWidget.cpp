// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventTaskWidget.h"
#include "moc_EventTaskWidget.cpp"
#include "ENT/ENT_Tr.h"
#include "gaming/Event.h"
#include <timeline/api.h>
#include <tools/Base64Converters.h>

// -----------------------------------------------------------------------------
// Name: EventTaskWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTaskWidget::EventTaskWidget()
    : EventWidget_ABC()
{
    label_ = new QLineEdit();
    description_ = new QTextEdit();
    url_ = new QLineEdit();
    apply_ = new QCheckBox( tr( "Apply" ) );
    apply_->setEnabled( false );
    bytes_ = new QLabel();
    bytes_->setEnabled( false );
    showButton_ = new QPushButton( tr( "Show" ) );
    connect( showButton_, SIGNAL( clicked() ), this, SLOT( OnShow() ) );
    payload_ = new QTextEdit();
    payload_->setFixedHeight( 50 );
    payload_->setVisible( false );

    QGridLayout* gridLayout = new QGridLayout( 3, 2 );
    gridLayout->setSpacing( 5 );
    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );

    QGroupBox* actionBox = new QGroupBox( tr( "Action" ) );
    QGridLayout* actionLayout = new QGridLayout( actionBox, 4, 3 );
    actionLayout->setColStretch( 1, 1 );
    actionLayout->setColStretch( 2, 1 );
    actionLayout->setMargin( 5 );
    actionLayout->setSpacing( 5 );
    actionLayout->addWidget( new QLabel( tr( "Url" ) ), 0, 0, Qt::AlignCenter );
    actionLayout->addWidget( url_, 0, 1, 1, 2 );
    actionLayout->addWidget( apply_, 1, 0, Qt::AlignCenter );
    actionLayout->addWidget( new QLabel( tr( "Payload" ) ), 2, 0, Qt::AlignCenter );
    actionLayout->addWidget( bytes_, 2, 1, Qt::AlignCenter );
    actionLayout->addWidget( showButton_, 2, 2, Qt::AlignCenter );
    actionLayout->addWidget( payload_, 4, 0, 1, 3, Qt::AlignCenter );

    mainLayout_->addLayout( gridLayout );
    mainLayout_->addWidget( actionBox );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTaskWidget::~EventTaskWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTaskWidget::Fill( const Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    label_->setText( QString::fromStdString( timelineEvent.name ) );
    description_->setText( QString::fromStdString( timelineEvent.info ) );
    url_->setText( QString::fromStdString( timelineEvent.action.target ) );
    apply_->setChecked( timelineEvent.action.apply );
    payload_->clear();
    bytes_->clear();
    if( !timelineEvent.action.payload.empty() )
    {
        std::string data = tools::BinaryToBase64( timelineEvent.action.payload );
        bytes_->setText( QString::number( data.size() ) + QString( " bytes" ) );
        payload_->setText( QString::fromStdString( data ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTaskWidget::Commit( timeline::Event& event ) const
{
    event.name = label_->text().toStdString();
    event.info = description_->text().toStdString();
    event.action.target = url_->text().toStdString();
    event.action.apply = apply_->checkState() == Qt::Checked;
    event.action.payload = tools::Base64ToBinary( payload_->text().toStdString() );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnShow
// Created: LGY 2013-11-18
// -----------------------------------------------------------------------------
void EventTaskWidget::OnShow()
{
    bool visible = payload_->isVisible();
    showButton_->setText( visible ? tr( "Show" ) : tr( "Hide" ) );
    payload_->setVisible( !visible );
}
