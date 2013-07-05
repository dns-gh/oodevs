// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventTopWidget.h"
#include "ENT/ENT_Tr.h"
#include "gaming/Event.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventTopWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTopWidget::EventTopWidget()
    : EventWidget_ABC()
{
    // Header
    QFont font( "Arial", 12, QFont::Bold );
    title_ = new QLabel();
    title_->setFont( font );
    source_ = new QLabel();
    source_->setFont( font );

    // Layout
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing( 5 );
    headerLayout->addWidget( title_ );
    headerLayout->addStretch( 1 );
    headerLayout->addWidget( source_ );
    mainLayout_->addLayout( headerLayout );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTopWidget::~EventTopWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTopWidget::Fill( const Event& event )
{
    title_->setText( QString::fromStdString( ENT_Tr::ConvertFromEventType( event.GetType() ) ) );
    source_->setText( QString::fromStdString( event.GetEvent().action.target ) );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTopWidget::Commit( timeline::Event& /* event */ ) const
{
    // NOTHING
}
