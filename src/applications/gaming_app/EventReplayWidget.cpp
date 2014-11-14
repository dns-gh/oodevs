// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventReplayWidget.h"
#include "moc_EventReplayWidget.cpp"

#include "clients_gui/EventReplayPresenter.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventReplayViewState.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichPathWidget.h"
#include "clients_gui/RichTextEdit.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: EventReplayWidget constructor
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
EventReplayWidget::EventReplayWidget( gui::EventPresenter& presenter )
    : EventReplayWidget_ABC( presenter )
{
    // Presenter
    replayPresenter_ = boost::make_shared< gui::EventReplayPresenter >( *this );
    presenter_.AddSubPresenter( replayPresenter_ );

    label_ = new gui::RichLineEdit( "replay-label" );
    description_ = new gui::RichTextEdit( "replay-description" );

    // Layout
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setSpacing( 5 );
    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable( true );
    QWidget* scrollWidget = new QWidget;
    scrollArea->setWidget( scrollWidget );
    QVBoxLayout* layout = new QVBoxLayout;
    scrollWidget->setLayout( layout );

    layout->addLayout( gridLayout, 1 );
    mainLayout_->addWidget( scrollArea );

    connect( label_, SIGNAL( textChanged( const QString& ) ), replayPresenter_.get(), SLOT( OnLabelChanged( const QString& ) ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), replayPresenter_.get(), SLOT( OnDescriptionChanged( const QString& ) ) );

    connect( label_, SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: EventReplayWidget destructor
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
EventReplayWidget::~EventReplayWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventReplayWidget::BlockSignals
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
void EventReplayWidget::BlockSignals( bool blocked )
{
    label_->blockSignals( blocked );
    description_->blockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: EventReplayWidget::Build
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
void EventReplayWidget::Build( const gui::EventReplayViewState& state )
{
    label_->SetText( state.label_ );
    description_->SetText( state.description_ );
}
