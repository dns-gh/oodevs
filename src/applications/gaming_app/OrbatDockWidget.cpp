// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "OrbatDockWidget.h"
#include "OrbatToolbar.h"
#include "CommunicationTreeView.h"
#include "LogisticListView.h" // à virer
#include "TacticalTreeView.h"
#include "clients_gui/AggregateToolBar.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/ObjectTreeView.h"
#include "clients_gui/SearchListView.h" // à virer
#include "clients_gui/SearchTreeView.h"
#include "gaming/ProfileFilter.h"

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget constructor
// Created: JSR 2012-09-26
// -----------------------------------------------------------------------------
OrbatDockWidget::OrbatDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName, const QString& windowTitle, 
                                  ProfileFilter& filter, gui::AutomatsLayer& automats, gui::FormationLayer& formations,
                                  actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation,
                                  gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons )
    : QDockWidget( parent )
{
    setWindowTitle( windowTitle );
    setObjectName( objectName );
    
    Q3VBox* box = new Q3VBox( this );
    setWidget( box );
    OrbatToolbar* orbatToolbar = new OrbatToolbar( box, controllers, filter, automats, formations );
    const gui::AggregateToolbar* aggregateToolbar = orbatToolbar->GetToolbar();
    QTabWidget* pListsTabWidget = new QTabWidget( box );

    gui::SearchTreeView_ABC* searchTreeView = 0;
    {
        searchTreeView = new gui::SearchTreeView< TacticalTreeView >( pListsTabWidget, controllers, filter, observer_, icons, staticModel, simulation, actionsModel );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tr( "Tactical" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< CommunicationTreeView >( pListsTabWidget, controllers, filter, observer_, icons, staticModel, simulation, actionsModel );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tr( "Communication" ) );
    }
    {
        gui::SearchListView< LogisticListView >* logisticSearchListView = new gui::SearchListView< ::LogisticListView >( pListsTabWidget, controllers, factory, filter, icons, actionsModel, staticModel, simulation );
        logisticSearchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), logisticSearchListView->GetRichListView(), SLOT( LockDragAndDrop( bool ) ) );
        logisticListView_ = logisticSearchListView->GetListView();
        pListsTabWidget->addTab( logisticSearchListView, tr( "Logistic" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< gui::ObjectTreeView >( pListsTabWidget, controllers, filter, observer_ );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tr( "Objects" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< gui::PopulationTreeView >( pListsTabWidget, controllers, filter, observer_ );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tr( "Crowds" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< gui::InhabitantTreeView >( pListsTabWidget, controllers, filter, observer_ );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tr( "Populations" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget destructor
// Created: JSR 2012-09-26
// -----------------------------------------------------------------------------
OrbatDockWidget::~OrbatDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::Purge
// Created: JSR 2012-09-26
// -----------------------------------------------------------------------------
void OrbatDockWidget::Purge()
{
    if( logisticListView_ )
        logisticListView_->Purge();
}
