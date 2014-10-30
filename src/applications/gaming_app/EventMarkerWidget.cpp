// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventMarkerWidget.h"
#include "moc_EventMarkerWidget.cpp"

#include "clients_gui/EventMarkerPresenter.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventMarkerViewState.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichPathWidget.h"
#include "clients_gui/RichTextEdit.h"
#include <boost/make_shared.hpp>


// -----------------------------------------------------------------------------
// Name: EventMarkerWidget constructor
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
EventMarkerWidget::EventMarkerWidget( gui::EventPresenter& presenter, const tools::Path& exercisePath )
    : EventMarkerWidget_ABC( presenter )
{
    // Presenter
    markerPresenter_ = boost::make_shared< gui::EventMarkerPresenter >( *this );
    presenter_.AddSubPresenter( markerPresenter_ );

    label_ = new gui::RichLineEdit( "marker-label" );
    description_ = new gui::RichTextEdit( "marker-description" );

    // Layout
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setSpacing( 5 );
    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1 );
    gridLayout->addWidget( new QLabel( tr( "Description" ) ), 1, 0 );
    gridLayout->addWidget( description_, 1, 1 );

    QGroupBox* drawingsBox = new QGroupBox( tr( "Drawings" ) );
    QVBoxLayout* drawingsLayout = new QVBoxLayout;
    drawingsBox->setLayout( drawingsLayout );
    resetDrawings_ = new gui::RichCheckBox( "reset-drawings", tr( "Clear previous drawings" ) );
    drawingsPath_ = new gui::RichPathWidget( "drawings-path", tr( "Load drawing file" ), exercisePath, tr( "Drawings (*.xml)" ) );
    drawingsLayout->addWidget( resetDrawings_ );
    drawingsLayout->addWidget( drawingsPath_ );

    QGroupBox* configurationBox = new QGroupBox( tr( "Display settings" ) );
    QVBoxLayout* configurationLayout = new QVBoxLayout;
    configurationBox->setLayout( configurationLayout );
    configurationPath_ = new gui::RichPathWidget( "configuration-path", tr( "Load configuration file" ), exercisePath, tr( "Configuration file (*.ini)" ) );
    configurationLayout->addWidget( configurationPath_ );

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable( true );
    QWidget* scrollWidget = new QWidget;
    scrollArea->setWidget( scrollWidget );
    QVBoxLayout* layout = new QVBoxLayout;
    scrollWidget->setLayout( layout );

    layout->addLayout( gridLayout, 1 );
    layout->addWidget( drawingsBox );
    layout->addWidget( configurationBox );

    mainLayout_->addWidget( scrollArea );

    connect( label_, SIGNAL( textChanged( const QString& ) ), markerPresenter_.get(), SLOT( OnLabelChanged( const QString& ) ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), markerPresenter_.get(), SLOT( OnDescriptionChanged( const QString& ) ) );
    connect( resetDrawings_, SIGNAL( toggled( bool ) ), markerPresenter_.get(), SLOT( OnResetDrawingsChanged( bool ) ) );
    connect( drawingsPath_, SIGNAL( PathChanged( const tools::Path& ) ), markerPresenter_.get(), SLOT( OnDrawingsPathChanged( const tools::Path& ) ) );
    connect( configurationPath_, SIGNAL( PathChanged( const tools::Path& ) ), markerPresenter_.get(), SLOT( OnConfigurationPathChanged( const tools::Path& ) ) );

    connect( label_, SIGNAL( textChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( description_, SIGNAL( TextChanged( const QString& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( resetDrawings_, SIGNAL( toggled( bool ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( drawingsPath_, SIGNAL( PathChanged( const tools::Path& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
    connect( configurationPath_, SIGNAL( PathChanged( const tools::Path& ) ), &presenter_, SLOT( OnEventContentChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: EventMarkerWidget destructor
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
EventMarkerWidget::~EventMarkerWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMarkerWidget::BlockSignals
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
void EventMarkerWidget::BlockSignals( bool blocked )
{
    label_->blockSignals( blocked );
    description_->blockSignals( blocked );
    resetDrawings_->blockSignals( blocked );
    drawingsPath_->blockSignals( blocked );
    configurationPath_->blockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: EventMarkerWidget::Build
// Created: JSR 2014-10-27
// -----------------------------------------------------------------------------
void EventMarkerWidget::Build( const gui::EventMarkerViewState& state )
{
    label_->SetText( QString::fromStdString( state.label_ ) );
    description_->SetText( QString::fromStdString( state.description_ ) );
    resetDrawings_->setChecked( state.resetDrawings_ );
    drawingsPath_->SetPath( state.drawingsPath_ );
    configurationPath_->SetPath( state.configurationPath_ );
}
