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
#include "ENT/ENT_Tr.h"
#include "gaming/Event.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventTaskWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTaskWidget::EventTaskWidget()
    : EventWidget_ABC()
{
    label_ = new QLineEdit();
    description_ = new QTextEdit();
    profiles_ = new QComboBox();

    QGridLayout* gridLayout = new QGridLayout( 3, 2 );
    gridLayout->setMargin( 10 );
    gridLayout->setSpacing( 10 );

    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );
    gridLayout->addWidget( new QLabel( tr( "Profiles" ) ), 2, 0 );
    gridLayout->addWidget( profiles_, 2, 1 );

    mainLayout_->addLayout( gridLayout );
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
    label_->setText( QString::fromStdString( event.GetEvent().name ) );
    description_->setText( QString::fromStdString( event.GetEvent().info ) );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTaskWidget::Commit( timeline::Event& event ) const
{
    event.name = label_->text().toStdString();
    event.info = description_->text().toStdString();
}
