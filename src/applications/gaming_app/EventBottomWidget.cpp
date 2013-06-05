// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventBottomWidget.h"
#include "moc_EventBottomWidget.cpp"
#include "gaming/Event.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventBottomWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventBottomWidget::EventBottomWidget()
    : EventWidget_ABC()
{
    QToolBar* toolbar = new QToolBar();

    toolbar->addAction( qApp->style()->standardIcon( QStyle::SP_MediaPlay ), tr( "Trigger" ), this, SIGNAL( Trigger() ) );
    flagAsCompleteAction_ = toolbar->addAction( qApp->style()->standardIcon( QStyle::SP_DialogApplyButton ), tr( "Flag as complete" ) );
    flagAsCompleteAction_->setCheckable( true );
    toolbar->addAction( qApp->style()->standardIcon( QStyle::SP_ComputerIcon ), tr( "Create a reminder" ), this, SIGNAL( CreateReminder() ) );
    detailAction_ = toolbar->addAction( qApp->style()->standardIcon( QStyle::SP_FileDialogInfoView ), tr( "Detail" ), this, SIGNAL( ShowDetail() ) );
    detailAction_->setCheckable( true );

    QWidget* empty = new QWidget();
    empty->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
    toolbar->addWidget( empty );

    toolbar->addAction( qApp->style()->standardIcon( QStyle::SP_BrowserReload ), tr( "Discard" ), this, SIGNAL( Discard() ) );
    toolbar->addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), tr( "Save" ), this, SIGNAL( Save() ) );

    mainLayout_->addWidget( toolbar );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventBottomWidget::~EventBottomWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventBottomWidget::Fill( const Event& event )
{
    flagAsCompleteAction_->setChecked( event.GetEvent().done );
    detailAction_->setChecked( false );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventBottomWidget::Commit( timeline::Event& event ) const
{
    event.done = flagAsCompleteAction_->isChecked();
}
