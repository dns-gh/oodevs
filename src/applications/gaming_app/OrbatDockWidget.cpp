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
#include "CommunicationTreeView.h"
#include "DrawingsBuilder.h"
#include "LogisticTreeView.h"
#include "OrbatToolbar.h"
#include "PopulationTreeView.h"
#include "TacticalTreeView.h"
#include "clients_gui/AggregateToolBar.h"
#include "clients_gui/DrawingsTreeView.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/ObjectTreeView.h"
#include "clients_gui/RichView.h"
#include "clients_kernel/Tools.h"
#include "gaming/ProfileFilter.h"

namespace
{
    template< typename View >
    View* Configure( QTabWidget* tabWidget, const QObject* toolbar, const QString& tabName, View* view )
    {
        QObject::connect( toolbar, SIGNAL( LockDragAndDrop( bool ) ), view->GetView(), SLOT( LockDragAndDrop( bool ) ) );
        tabWidget->addTab( view, tabName );
        return view;
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget constructor
// Created: JSR 2012-09-26
// -----------------------------------------------------------------------------
OrbatDockWidget::OrbatDockWidget( kernel::Controllers& controllers,
                                  QWidget* parent,
                                  const QString& objectName,
                                  ProfileFilter& filter,
                                  gui::AutomatsLayer& automats,
                                  gui::FormationLayer& formations,
                                  actions::ActionsModel& actionsModel,
                                  const StaticModel& staticModel,
                                  const kernel::Time_ABC& simulation,
                                  const gui::EntitySymbols& icons,
                                  DrawingsBuilder& drawingsBuilder,
                                  gui::ParametersLayer& paramLayer )
    : gui::RichDockWidget( controllers, parent, objectName )
    , logisticListView_( 0 )
{
    setWindowTitle( tools::translate( "OrbatDockWidget", "Orbat" ) );

    Q3VBox* box = new Q3VBox( this );
    setWidget( box );
    OrbatToolbar* orbatToolbar = new OrbatToolbar( box, controllers, filter, automats, formations );
    const gui::AggregateToolbar* toolbar = orbatToolbar->GetToolbar();
    QTabWidget* mainTab = new QTabWidget( box );
    QTabWidget* unitTab = new QTabWidget();
    mainTab->addTab( unitTab, tools::translate( "OrbatDockWidget", "Units" ) );
    // Tactical
    auto tactical = Configure( unitTab, toolbar, tools::translate( "OrbatDockWidget", "Tactical" ),
                               new gui::RichView< TacticalTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                      "SearchTacticalTreeView",
                                                                      unitTab,
                                                                      controllers,
                                                                      filter,
                                                                      observer_,
                                                                      icons,
                                                                      staticModel,
                                                                      simulation,
                                                                      actionsModel ) );
    connect( toolbar, SIGNAL( ChangeDisplay( int ) ), tactical->GetView(), SLOT( ChangeDisplay( int ) ) );
    // Communication
    Configure( unitTab, toolbar, tools::translate( "OrbatDockWidget", "Communication" ),
                new gui::RichView< CommunicationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                            "SearchCommunicationTreeView",
                                                            unitTab,
                                                            controllers,
                                                            filter,
                                                            observer_,
                                                            icons,
                                                            staticModel,
                                                            simulation,
                                                            actionsModel ) );
    // Logistic
    auto logistic = Configure( unitTab, toolbar, tools::translate( "OrbatDockWidget", "Logistic" ),
                               new gui::RichView< LogisticTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                      "SearchLogisticTreeView",
                                                                      mainTab,
                                                                      controllers,
                                                                      filter,
                                                                      observer_,
                                                                      icons,
                                                                      staticModel,
                                                                      simulation,
                                                                      actionsModel ) );
    logisticListView_ = logistic->GetView();
    // Object
    Configure( mainTab, toolbar, tools::translate( "OrbatDockWidget", "Objects" ),
               new gui::RichView< gui::ObjectTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                         "ObjectTreeView",
                                                         mainTab,
                                                         controllers,
                                                         filter,
                                                         observer_ ) );
    // Crowds
    Configure( mainTab, toolbar, tools::translate( "OrbatDockWidget", "Crowds" ),
               new gui::RichView< PopulationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                        "PopulationTreeView",
                                                        mainTab,
                                                        controllers,
                                                        filter,
                                                        observer_ ) );
    // Populations
    Configure( mainTab, toolbar, tools::translate( "OrbatDockWidget", "Populations" ),
               new gui::RichView< gui::InhabitantTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                             "InhabitantTreeView",
                                                             mainTab,
                                                             controllers,
                                                             filter,
                                                             observer_ ) );
    // Populations
    Configure( mainTab, toolbar, tools::translate( "OrbatDockWidget", "Drawings" ),
               new gui::RichView< gui::InhabitantTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                             "DrawingsTreeView",
                                                             mainTab,
                                                             controllers,
                                                             filter,
                                                             drawingsBuilder,
                                                             paramLayer ) );
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
    logisticListView_->Purge();
}
