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
#include "clients_gui/Event.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLabel.h"
#include "tools/ExerciseConfig.h"
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
EventBottomWidget::EventBottomWidget( gui::EventPresenter& presenter )
    : EventDefaultWidget_ABC( presenter )
    , detailAction_( 0 )
{
    // Engagement warning
    misengageBox_ = new gui::RichGroupBox( "misengage-box" );
    misengageLabel_ = new gui::RichLabel( "misengage_label" );
    QVBoxLayout* misengageLayout = new QVBoxLayout( misengageBox_ );
    misengageLayout->addWidget( misengageLabel_, 0, Qt::AlignCenter );

    // Warning
    warningBox_ = new gui::RichGroupBox( "warning-box" );
    warningLabel_ = new gui::RichLabel( "acknowledged_label" );
    QVBoxLayout* warningLayout = new QVBoxLayout( warningBox_ );
    warningLayout->addWidget( warningLabel_, 0, Qt::AlignCenter );

    // Trigger Button
    triggerButton_ = new QToolButton();
    triggerButton_->setIcon( qApp->style()->standardIcon( QStyle::SP_MediaPlay ) );
    triggerButton_->setFixedSize( QSize( 80, 40 ) );
    connect( triggerButton_, SIGNAL( clicked() ), &presenter, SLOT( OnTriggerClicked() ) );

    // ToolBar
    toolBar_ = new QToolBar();
    toolBar_->setStyleSheet("QToolBar { border: 0px }");
    toolBar_->setIconSize( QSize( 30, 30 ) );
    triggerAction_ = toolBar_->addWidget( triggerButton_ );
    toolBar_->addWidget( CreateStretcher() );
    clearAction_ = toolBar_->addAction( qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ),
                                         tr( "Clear parameters" ),
                                         &presenter, SLOT( OnClearClicked() ) );
    detailAction_ = toolBar_->addAction( qApp->style()->standardIcon( QStyle::SP_FileDialogInfoView ),
                                        tr( "Show/hide details" ),
                                        &presenter, SLOT( OnDetailClicked() ) );
    detailAction_->setCheckable( true );
#ifndef _DEBUG
    detailAction_->setVisible( false );
#endif

    // Layout
    mainLayout_->addWidget( misengageBox_ );
    mainLayout_->addWidget( warningBox_ );
    mainLayout_->addWidget( toolBar_ );
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
// Name: EventBottomWidget::Purge
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventBottomWidget::Purge()
{
    misengageBox_->setVisible( false );
    misengageBox_->EnableStaticWarning( false );
    misengageLabel_->setText( "" );
    misengageLabel_->EnableStaticWarning( false );
    warningBox_->setVisible( false );
    warningBox_->EnableStaticWarning( false );
    warningLabel_->setText( "" );
    warningLabel_->EnableStaticWarning( false );
    triggerButton_->setToolTip( "" );
    triggerButton_->setEnabled( false );
    clearAction_->setEnabled( false );
    detailAction_->setChecked( false );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::Build
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventBottomWidget::Build( const gui::EventViewState& state )
{
    toolBar_->setVisible( state.bottomToolBar_ || detailAction_->isVisible() );
    misengageBox_->setVisible( state.misengaged_ );
    if( state.misengaged_ )
    {
        static const auto color = QColor( "#FF7700" );
        misengageBox_->EnableStaticWarning( true, color );
        misengageLabel_->setText( *state.misengaged_ ?
            tr( "Automat is engaged" ) : tr( "Automat is disengaged" ) );
        misengageLabel_->EnableStaticWarning( true, color );
    }
    warningBox_->setVisible( !state.warning_.empty() );
    warningBox_->EnableStaticWarning( true, state.warningColor_ );
    warningLabel_->setText( QString::fromStdString( state.warning_ ) );
    warningLabel_->EnableStaticWarning( true, state.warningColor_ );
    clearAction_->setVisible( state.bottomToolBar_ );
    clearAction_->setEnabled( state.clear_ );
    triggerAction_->setVisible( state.bottomToolBar_ );
    triggerButton_->setEnabled( state.trigger_ );
    triggerButton_->setToolTip( state.mode_ == eEventDockModes_DisplayTriggered
                                ? tr( "Copy and trigger" )
                                : tr( "Trigger" ) );
    detailAction_->setChecked( state.detail_ );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::BlockSignals
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventBottomWidget::BlockSignals( bool blocked )
{
    detailAction_->blockSignals( blocked );
    clearAction_->blockSignals( blocked );
    triggerButton_->blockSignals( blocked );
}
