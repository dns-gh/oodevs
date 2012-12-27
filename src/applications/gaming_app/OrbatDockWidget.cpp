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
#include "TacticalTreeView.h"
#include "LogisticTreeView.h"
#include "clients_gui/AggregateToolBar.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/ObjectTreeView.h"
#include "clients_gui/SearchTreeView.h"
#include "clients_kernel/Tools.h"
#include "gaming/ProfileFilter.h"

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget constructor
// Created: JSR 2012-09-26
// -----------------------------------------------------------------------------
OrbatDockWidget::OrbatDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName, const QString& windowTitle,
                                  ProfileFilter& filter, gui::AutomatsLayer& automats, gui::FormationLayer& formations,
                                  actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation,
                                  gui::EntitySymbols& icons )
    : QDockWidget( parent )
    , logisticListView_( 0 )
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
        QTabWidget* pUnit = new QTabWidget();
        {
            searchTreeView = new gui::SearchTreeView< TacticalTreeView >( pUnit, controllers, filter, observer_, icons, staticModel, simulation, actionsModel );
            searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
            pUnit->addTab( searchTreeView, tools::translate( "OrbatDockWidget", "Tactical" ) );
        }
        {
            searchTreeView = new gui::SearchTreeView< CommunicationTreeView >( pUnit, controllers, filter, observer_, icons, staticModel, simulation, actionsModel );
            searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
            pUnit->addTab( searchTreeView, tools::translate( "OrbatDockWidget", "Communication" ) );
        }
        {
            gui::SearchTreeView< LogisticTreeView >* logisticSearchListView = new gui::SearchTreeView< LogisticTreeView >( pListsTabWidget, controllers, filter, observer_, icons, staticModel, simulation, actionsModel );
            logisticSearchListView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), logisticSearchListView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
            logisticListView_ = logisticSearchListView->GetTreeView();
            pUnit->addTab( logisticSearchListView, tools::translate( "OrbatDockWidget", "Logistic" ) );
        }
        pListsTabWidget->addTab( pUnit, tools::translate( "OrbatDockWidget", "Units" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< gui::ObjectTreeView >( pListsTabWidget, controllers, filter, observer_ );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tools::translate( "OrbatDockWidget", "Objects" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< gui::PopulationTreeView >( pListsTabWidget, controllers, filter, observer_ );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tools::translate( "OrbatDockWidget", "Crowds" ) );
    }
    {
        searchTreeView = new gui::SearchTreeView< gui::InhabitantTreeView >( pListsTabWidget, controllers, filter, observer_ );
        searchTreeView->connect( aggregateToolbar, SIGNAL( LockDragAndDrop( bool ) ), searchTreeView->GetRichTreeView(), SLOT( LockDragAndDrop( bool ) ) );
        pListsTabWidget->addTab( searchTreeView, tools::translate( "OrbatDockWidget", "Populations" ) );
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
