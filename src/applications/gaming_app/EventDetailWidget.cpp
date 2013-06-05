// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventDetailWidget.h"
#include "gaming/Event.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventDetailWidget constructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
EventDetailWidget::EventDetailWidget()
{
    uuid_ = new QLineEdit();
    name_ = new QLineEdit();
    info_ = new QTextEdit();
    begin_ = new QLineEdit();
    end_ = new QLineEdit();
    done_ = new QCheckBox();
    target_ = new QLineEdit();
    apply_ = new QCheckBox();
    payload_ = new QTextEdit();

    QGridLayout* gridLayout = new QGridLayout( 10, 2 );
    mainLayout_->addLayout( gridLayout );
    gridLayout->setMargin( 10 );
    gridLayout->setSpacing( 10 );
    gridLayout->addWidget( new QLabel( "UUID" ), 0, 0 );
    gridLayout->addWidget( uuid_, 0, 1 );
    gridLayout->addWidget( new QLabel( "Name" ), 1, 0 );
    gridLayout->addWidget( name_, 1, 1 );
    gridLayout->addWidget( new QLabel( "Infos" ), 2, 0 );
    gridLayout->addWidget( info_, 2, 1 );
    gridLayout->addWidget( new QLabel( "Begin" ), 3, 0 );
    gridLayout->addWidget( begin_, 3, 1 );
    gridLayout->addWidget( new QLabel( "End" ), 4, 0 );
    gridLayout->addWidget( end_, 4, 1 );
    gridLayout->addWidget( new QLabel( "Done" ), 5, 0 );
    gridLayout->addWidget( done_, 5, 1 );
    gridLayout->addWidget( new QLabel( "Action" ), 6, 0, 1, 2 );
    gridLayout->addWidget( new QLabel( "Target" ), 7, 0 );
    gridLayout->addWidget( target_, 7, 1 );
    gridLayout->addWidget( new QLabel( "Apply" ), 8, 0 );
    gridLayout->addWidget( apply_, 8, 1 );
    gridLayout->addWidget( new QLabel( "Payload" ), 9, 0 );
    gridLayout->addWidget( payload_, 9, 1 );
}

// -----------------------------------------------------------------------------
// Name: EventDetailWidget destructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
EventDetailWidget::~EventDetailWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventDetailWidget::Fill
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventDetailWidget::Fill( const Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    uuid_->setText( QString::fromStdString( timelineEvent.uuid ) );
    name_->setText( QString::fromStdString( timelineEvent.name ) );
    info_->setText( QString::fromStdString( timelineEvent.info ) );
    begin_->setText( QString::fromStdString( timelineEvent.begin ) );
    end_->setText( QString::fromStdString( timelineEvent.end ) );
    done_->setCheckState( timelineEvent.done ? Qt::Checked : Qt::Unchecked );
    target_->setText( QString::fromStdString( timelineEvent.action.target ) );
    apply_->setCheckState( timelineEvent.action.apply ? Qt::Checked : Qt::Unchecked );
    payload_->setText( QString::fromStdString( timelineEvent.action.payload ) );
}

// -----------------------------------------------------------------------------
// Name: EventDetailWidget::Commit
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventDetailWidget::Commit( timeline::Event& event ) const
{
    event.uuid = uuid_->text().toStdString();
    event.name = name_->text().toStdString();
    event.info = info_->text().toStdString();
    event.begin = begin_->text().toStdString();
    event.end = end_->text().toStdString();
    event.done = done_->checkState() == Qt::Checked;
    event.action.target = target_->text().toStdString();
    event.action.apply = apply_->checkState() == Qt::Checked;
    event.action.payload = payload_->text().toStdString();
}
