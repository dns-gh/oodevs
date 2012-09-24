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
#include "ListViewsPanel.h"
#include "TreeViewsPanel.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_kernel/Controllers.h"
#include "ENT/ENT_Enums_Gen.h"

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget constructor
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
OrbatDockWidget::OrbatDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName,
                                  const QString& windowTitle, gui::AutomatsLayer& automats, gui::FormationLayer& formation,
                                  gui::EntitySymbols& icons, ModelBuilder& modelBuilder, gui::ItemFactory_ABC& factory,
                                  Model& model, StaticModel& staticModel, std::vector< gui::SearchListView_ABC* >& listViews, gui::SymbolIcons& symbols )
    : gui::RichDockWidget( controllers, parent, objectName, windowTitle )
    , expandIcon_    ( QIcon( "resources/images/preparation/double_arrow_right.png" ) )
    , collapseIcon_  ( QIcon( "resources/images/preparation/double_arrow_left.png" ) )
    , expanded_      ( false )
    , oldMinWith_    ( 0 )
    , oldMaxWith_    ( 0 )
    , pListViewPanel_( 0 )
    , pTreeViewPanel_( 0 )
{
    QGridLayout* toolbarBox = new QGridLayout();
    gui::AggregateToolbar* aggregateToolbar = new gui::AggregateToolbar( controllers.controller_, automats, formation );
    toolbarBox->addLayout( aggregateToolbar, 0, 0, 1, 1, Qt::AlignLeft );

    expandButton_ = new QToolButton();
    expandButton_->setFixedSize( 20, 20 );
    expandButton_->setAutoRaise( true );
    expandButton_->setIcon( expandIcon_ );
    toolbarBox->addWidget( expandButton_, 0, 1, 1, 1, Qt::AlignRight );

    pListViewPanel_ = new ListViewsPanel( controllers, factory, icons, modelBuilder, model, listViews, symbols, staticModel, *aggregateToolbar );

    QWidget* widget = new QWidget();
    QVBoxLayout* box = new QVBoxLayout( widget );
    box->setSpacing( 0 );
    box->setMargin( 0 );
    box->addLayout( toolbarBox );
    box->addWidget( pListViewPanel_, 1 );

    setWidget( widget );

    connect( expandButton_, SIGNAL( clicked() ), this, SLOT( OnCollapseClicked() ) );
    connect( pListViewPanel_, SIGNAL( currentChanged( int ) ), this, SLOT( OnTabChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget constructor
// Created: LGY 2012-06-27
// -----------------------------------------------------------------------------
OrbatDockWidget::OrbatDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName,
    const QString& windowTitle, gui::AutomatsLayer& automats, gui::FormationLayer& formation,
    gui::EntitySymbols& icons, ModelBuilder& modelBuilder, Model& model, StaticModel& staticModel, std::vector< gui::SearchTreeView_ABC* >& treeViews, gui::SymbolIcons& symbols )
    : gui::RichDockWidget( controllers, parent, objectName, windowTitle )
    , pListViewPanel_( 0 )
    , pTreeViewPanel_( 0 )
    , expandIcon_  ( QIcon( "resources/images/preparation/double_arrow_right.png" ) )
    , collapseIcon_( QIcon( "resources/images/preparation/double_arrow_left.png" ) )
    , expanded_    ( false )
    , oldMinWith_  ( 0 )
    , oldMaxWith_  ( 0 )
{
    QGridLayout* toolbarBox = new QGridLayout();
    gui::AggregateToolbar* aggregateToolbar = new gui::AggregateToolbar( controllers.controller_, automats, formation );
    toolbarBox->addLayout( aggregateToolbar, 0, 0, 1, 1, Qt::AlignLeft );

    expandButton_ = new QToolButton();
    expandButton_->setFixedSize( 20, 20 );
    expandButton_->setAutoRaise( true );
    expandButton_->setIcon( expandIcon_ );
    toolbarBox->addWidget( expandButton_, 0, 1, 1, 1, Qt::AlignRight );

    pTreeViewPanel_ = new TreeViewsPanel( controllers, icons, modelBuilder, model, treeViews, symbols, staticModel, *aggregateToolbar );

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
    if( pListViewPanel_ )
        pListViewPanel_->SetVisible( value );
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
void OrbatDockWidget::NotifyModeChanged( int newMode, bool useDefault, bool firstChangeToSavedMode )
{
    expandButton_->setEnabled( newMode == ePreparationMode_Exercise );
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
