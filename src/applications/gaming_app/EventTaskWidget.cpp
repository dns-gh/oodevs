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
    connect( url_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnUrlChanged() ) );
    bytes_ = new QLabel();
    bytes_->setEnabled( false );
    showButton_ = new QPushButton( tr( "Show" ) );
    connect( showButton_, SIGNAL( clicked() ), this, SLOT( OnShow() ) );
    payload_ = new QTextEdit();
    payload_->setVisible( false );
    payloadLabel_ = new QLabel( tr( "Base64 Payload" ) );

    QGridLayout* gridLayout = new QGridLayout( 3, 2 );
    gridLayout->setSpacing( 5 );
    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );

    QGroupBox* actionBox = new QGroupBox( tr( "Action" ) );
    QVBoxLayout* actionLayout = new QVBoxLayout( actionBox );
    QHBoxLayout* urlLayout = new QHBoxLayout( actionLayout );
    urlLayout->addWidget( new QLabel( tr( "Url" ) ) );
    urlLayout->addWidget( url_ );

    QHBoxLayout* payloadLayout = new QHBoxLayout( actionLayout );

    payloadLayout->addWidget( payloadLabel_ );
    payloadLayout->addWidget( bytes_ );
    payloadLayout->addWidget( showButton_ );

    actionLayout->addWidget( payload_ );

    mainLayout_->addLayout( gridLayout );
    mainLayout_->addWidget( actionBox );
    OnUrlChanged();
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
    payload_->clear();
    bytes_->clear();
    if( !timelineEvent.action.payload.empty() && !url_->text().isEmpty() )
    {
        QString data = QByteArray::fromBase64( timelineEvent.action.payload.c_str() );
        bytes_->setText( QString::number( data.size() ) + QString( " bytes" ) );
        payload_->setText( data );
    }
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTaskWidget::Commit( timeline::Event& event )
{
    event.name = label_->text().toStdString();
    event.info = description_->text().toStdString();
    event.action.target = url_->text().toStdString();
    event.action.apply = false;
    if( !event.action.target.empty() )
    {
        QByteArray data( payload_->text().toStdString().c_str() );
        event.action.payload = data.toBase64().data();
    }
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

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnUrlChanged
// Created: LGY 2013-11-18
// -----------------------------------------------------------------------------
void EventTaskWidget::OnUrlChanged()
{
    bool valid = !url_->text().isEmpty();
    showButton_->setVisible( valid );
    bytes_->setVisible( valid );
    payloadLabel_->setVisible( valid );
    if( !valid )
        payload_->setVisible( valid );
}
