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
#include <timeline/api.h>

namespace
{
    QWidget* CreateStretcher()
    {
        QWidget* empty = new QWidget();
        empty->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        return empty;
    }
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventBottomWidget::EventBottomWidget()
    : EventWidget_ABC()
    , detailAction_( 0 )
{
    // Trigger Button
    triggerButton_ = new QToolButton();
    triggerButton_->setIcon( qApp->style()->standardIcon( QStyle::SP_MediaPlay ) );
    triggerButton_->setFixedSize( QSize( 80, 40 ) );
    connect( triggerButton_, SIGNAL( clicked() ), this, SIGNAL( Trigger() ) );

    // ToolBar
    QToolBar* toolBar = new QToolBar();
    toolBar->setStyleSheet("QToolBar { border: 0px }");
    toolBar->setIconSize( QSize( 30, 30 ) );
    toolBar->addWidget( triggerButton_ );
    toolBar->addWidget( CreateStretcher() );
    toolBar->addAction( qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), tr( "Discard" ), this, SIGNAL( Discard() ) );

    // Details if debug
#ifdef _DEBUG
        detailAction_ = toolBar->addAction( qApp->style()->standardIcon( QStyle::SP_FileDialogInfoView ), "Details", this, SIGNAL( ShowDetail() ) );
        detailAction_->setCheckable( true );
#endif

    // Layout
    mainLayout_->addWidget( toolBar );
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
    triggerButton_->setToolTip( event.GetEvent().done ? tr( "Copy and trigger" ) : tr( "Trigger" ) );
    if( detailAction_ )
        detailAction_->setChecked( false );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventBottomWidget::Commit( timeline::Event& /* event */ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnEnableTriggerEvent
// Created: NPT 2013-09-04
// -----------------------------------------------------------------------------
void EventBottomWidget::OnEnableTriggerEvent( bool enable )
{
    triggerButton_->setEnabled( enable );
}
