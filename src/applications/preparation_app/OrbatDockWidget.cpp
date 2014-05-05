// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "OrbatDockWidget.h"
#include "moc_OrbatDockWidget.cpp"
#include "TreeViewsPanel.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichWidget.h"
#include "clients_kernel/Controllers.h"
#include "ENT/ENT_Enums.h"

#define ICON_RIGHT gui::Icon( "resources/images/preparation/double_arrow_right.png" )
#define ICON_LEFT  gui::Icon( "resources/images/preparation/double_arrow_left.png" )

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget constructor
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
OrbatDockWidget::OrbatDockWidget( kernel::Controllers& controllers,
                                  QWidget* parent,
                                  const QString& objectName,
                                  const QString& windowTitle,
                                  gui::AutomatsLayer& automats,
                                  gui::FormationLayer& formation,
                                  gui::EntitySymbols& icons,
                                  ModelBuilder& modelBuilder,
                                  Model& model,
                                  StaticModel& staticModel,
                                  std::vector< gui::RichView_ABC* >& views,
                                  gui::SymbolIcons& symbols,
                                  gui::ParametersLayer& paramLayer )
    : gui::RichDockWidget( controllers, parent, objectName, windowTitle )
    , pTreeViewPanel_( 0 )
    , expandIcon_  ( qApp->layoutDirection() == Qt::RightToLeft ? ICON_LEFT : ICON_RIGHT )
    , collapseIcon_( qApp->layoutDirection() == Qt::RightToLeft ? ICON_RIGHT : ICON_LEFT )
    , expanded_    ( false )
    , oldMinWith_  ( 0 )
    , oldMaxWith_  ( 0 )
{
    gui::SubObjectName subObject( "OrbatDockWidget" );
    QGridLayout* toolbarBox = new QGridLayout();
    gui::AggregateToolbar* aggregateToolbar = new gui::AggregateToolbar( controllers.controller_, automats, formation, false );
    toolbarBox->addLayout( aggregateToolbar, 0, 0, 1, 1, Qt::AlignLeft );

    expandButton_ = new gui::RichWidget< QToolButton >( "expandButton" );
    expandButton_->setFixedSize( 20, 20 );
    expandButton_->setAutoRaise( true );
    expandButton_->setIcon( expandIcon_ );
    toolbarBox->addWidget( expandButton_, 0, 1, 1, 1, Qt::AlignRight );

    pTreeViewPanel_ = new TreeViewsPanel( controllers, icons, modelBuilder, model, views, symbols, staticModel, *aggregateToolbar, paramLayer );

    QWidget* widget = new QWidget();
    QVBoxLayout* box = new QVBoxLayout( widget );
    box->setSpacing( 0 );
    box->setMargin( 0 );
    box->addLayout( toolbarBox );
    box->addWidget( pTreeViewPanel_, 1 );

    setWidget( widget );

    connect( expandButton_, SIGNAL( clicked() ), this, SLOT( OnCollapseClicked() ) );
    connect( pTreeViewPanel_, SIGNAL( currentChanged( int ) ), this, SLOT( OnTabChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget destructor
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
OrbatDockWidget::~OrbatDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::OnCollapseClicked
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void OrbatDockWidget::OnCollapseClicked()
{
    Expand( !expanded_ );
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::Expand
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void OrbatDockWidget::Expand( bool value )
{
    expanded_ = value;
    expandButton_->setIcon( value ? collapseIcon_ : expandIcon_ );
    if( pTreeViewPanel_ )
        pTreeViewPanel_->SetVisible( value );

    oldMinWith_ = minimumWidth();
    oldMaxWith_ = maximumWidth();

    setFixedWidth( value ? width() * 2 : width() / 2 );
    QTimer::singleShot( 1, this, SLOT( RestoreWidth() ) );
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::RestoreWidth
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void OrbatDockWidget::RestoreWidth()
{
    setMinimumWidth( oldMinWith_ );
    setMaximumWidth( oldMaxWith_ );
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::NotifyModeChanged
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void OrbatDockWidget::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    expandButton_->setEnabled( newMode == eModes_Prepare );
    if( expanded_ )
        Expand( false );
    RichDockWidget::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::OnTabChanged
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
void OrbatDockWidget::OnTabChanged( int index )
{
    expandButton_->setEnabled( index == 0 );
    if( expanded_ )
        Expand( false );
}
