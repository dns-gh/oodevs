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
    label_ = new QLineEdit();
    description_ = new QTextEdit();
    url_ = new QLineEdit();
    bytes_ = new QLabel();
    bytes_->setEnabled( false );
    showButton_ = new QPushButton( tr( "Show" ) );
    payload_ = new QTextEdit();
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
    connect( description_, SIGNAL( textChanged() ),                 this,                 SLOT( OnDescriptionChanged() ) );
    connect( payload_,     SIGNAL( textChanged() ),                 this,                 SLOT( OnPayloadChanged() ) );
    // We want theses slots to be called after those from task presenter
    connect( label_,       SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( url_,         SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( description_, SIGNAL( textChanged() ),                 &presenter_, SLOT( OnEventContentChanged() ) );
    connect( payload_,     SIGNAL( textChanged() ),                 &presenter_, SLOT( OnEventContentChanged() ) );
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
// Name: EventTaskWidget::SaveCursor
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::SaveCursor( QTextEdit& textEdit )
{
    cursorPos_ = textEdit.textCursor().position();
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::RestoreCursor
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::RestoreCursor( QTextEdit& textEdit )
{
    if( cursorPos_ < 0 )
        return;
    QTextCursor cursor = textEdit.textCursor();
    cursor.setPosition( cursorPos_, QTextCursor::MoveAnchor );
    textEdit.setTextCursor( cursor );
    cursorPos_ = -1;
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnDescriptionChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::OnDescriptionChanged()
{
    SaveCursor( *description_ );
    taskPresenter_->OnDescriptionChanged( description_->text() );
    RestoreCursor( *description_ );
}

// -----------------------------------------------------------------------------
// Name: EventTaskWidget::OnPayloadChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskWidget::OnPayloadChanged()
{
    SaveCursor( *payload_ );
    taskPresenter_->OnPayloadChanged( payload_->text() );
    RestoreCursor( *payload_ );
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
    label_->setText( QString::fromStdString( state.label_ ) );
    description_->setText( QString::fromStdString( state.description_ ) );
    url_->setText( QString::fromStdString( state.url_ ) );
    payload_->clear();
    bytes_->clear();
    if( state.bytes_ != 0 )
        bytes_->setText( QString::number( state.bytes_ ) + QString( " bytes" ) );
    if( !state.payload_.empty() && !state.url_.empty() )
        payload_->setText( QString::fromStdString( state.payload_ ) );
    showButton_->setText( state.isPayloadVisible_ ? tr( "Hide" ) : tr( "Show" ) );
    showButton_->setVisible( state.isUrlValid_ );
    bytes_->setVisible( state.isUrlValid_ );
    payloadLabel_->setVisible( state.isUrlValid_ );
    payload_->setVisible( state.isUrlValid_ && state.isPayloadVisible_ );
}
