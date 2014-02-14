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
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventTaskPresenter.h"
#include "clients_gui/EventTaskViewState.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichTextEdit.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: EventTaskWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTaskWidget::EventTaskWidget( gui::EventPresenter& presenter )
    : EventTaskWidget_ABC( presenter )
{
    // Presenter
    taskPresenter_ = boost::make_shared< gui::EventTaskPresenter >( *this );
    presenter_.AddSubPresenter( taskPresenter_ );

    // Editors
    label_ = new gui::RichLineEdit( "task-label" );
    description_ = new gui::RichTextEdit( "task-description" );
    url_ = new gui::RichLineEdit( "task-url" );
    bytes_ = new QLabel();
    bytes_->setEnabled( false );
    showButton_ = new QPushButton( tr( "Show" ) );
    payload_ = new gui::RichTextEdit( "task-payload" );
    payload_->setVisible( false );
    payloadLabel_ = new QLabel( tr( "Base64 Payload" ) );

    // Layouts
    QGridLayout* gridLayout = new QGridLayout( 3, 2 );
    gridLayout->setSpacing( 5 );
    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );

    QGroupBox* actionBox = new QGroupBox( tr( "Action" ) );
    QVBoxLayout* actionLayout = new QVBoxLayout( actionBox );
    QHBoxLayout* urlLayout = new QHBoxLayout( actionLayout );
    QHBoxLayout* payloadLayout = new QHBoxLayout( actionLayout );

    urlLayout->addWidget( new QLabel( tr( "Url" ) ) );
    urlLayout->addWidget( url_ );

    payloadLayout->addWidget( payloadLabel_ );
    payloadLayout->addWidget( bytes_ );
    payloadLayout->addWidget( showButton_ );

    actionLayout->addWidget( payload_ );

    mainLayout_->addLayout( gridLayout );
    mainLayout_->addWidget( actionBox );

    // Connections
    connect( label_,       SIGNAL( textChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnLabelChanged( const QString& ) ) );
    connect( url_,         SIGNAL( textChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnUrlChanged( const QString& ) ) );
    connect( showButton_,  SIGNAL( clicked() ),                     taskPresenter_.get(), SLOT( OnShowClicked() ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnDescriptionChanged( const QString& ) ) );
    connect( payload_,     SIGNAL( TextChanged( const QString& ) ), taskPresenter_.get(), SLOT( OnPayloadChanged( const QString& ) ) );
    // We want theses slots to be called after those from task presenter
    connect( label_,       SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( url_,         SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( payload_,     SIGNAL( TextChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
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
// Name: EventTaskWidget::BlockSignals
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::BlockSignals( bool blocked )
{
    label_->blockSignals( blocked );
    description_->blockSignals( blocked );
    url_->blockSignals( blocked );
    payload_->blockSignals( blocked );
    bytes_->blockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::Build
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::Build( const gui::EventTaskViewState& state )
{
    label_->SetText( QString::fromStdString( state.label_ ) );
    description_->SetText( QString::fromStdString( state.description_ ) );
    url_->SetText( QString::fromStdString( state.url_ ) );
    bytes_->clear();
    if( state.bytes_ != 0 )
        bytes_->setText( QString::number( state.bytes_ ) + QString( " bytes" ) );
    payload_->SetText( QString::fromStdString( state.payload_ ) );
    showButton_->setText( state.isPayloadVisible_ ? tr( "Hide" ) : tr( "Show" ) );
    showButton_->setVisible( state.isUrlValid_ );
    bytes_->setVisible( state.isUrlValid_ );
    payloadLabel_->setVisible( state.isUrlValid_ );
    payload_->setVisible( state.isUrlValid_ && state.isPayloadVisible_ );
}
