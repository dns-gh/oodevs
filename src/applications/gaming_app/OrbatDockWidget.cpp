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
#include "actions/ActionsModel.h"
#include "clients_gui/AggregateToolBar.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_gui/DrawingsTreeView.h"
#include "clients_gui/DummyModelObserver.h"
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/ObjectTreeView.h"
#include "clients_gui/RenameInterface.h"
#include "clients_gui/RichView.h"
#include "clients_kernel/Tools.h"
#include "gaming/ProfileFilter.h"

#include "gaming/Agent.h"
#include "gaming/Automat.h"
#include "gaming/Formation.h"
#include "gaming/KnowledgeGroup.h"
#include "gaming/Object.h"
#include "clients_gui/UrbanObject.h"
#include "gaming/Population.h"
#include "gaming/Team.h"
#include "clients_gui/DummyModelObserver.h"

namespace
{
    template< typename View >
    View* Configure( std::vector< gui::RichView_ABC* >& views,
                     QTabWidget* tabWidget,
                     const QObject* toolbar,
                     const QString& tabName,
                     gui::RenameInterface& renameInterface,
                     View* richView )
    {
        auto view = richView->GetView();
        if( !view )
            return 0;
        QObject::connect( toolbar, SIGNAL( LockDragAndDrop( bool ) ), view, SLOT( LockDragAndDrop( bool ) ) );
        tabWidget->addTab( richView, tabName );
        views.push_back( richView );
        renameInterface.AddEntityTreeView( view );
        return richView;
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
                                  const std::shared_ptr< gui::AutomatsLayer >& automats,
                                  const std::shared_ptr< gui::FormationLayer >& formations,
                                  const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                                  actions::ActionsModel& actionsModel,
                                  const StaticModel& staticModel,
                                  const kernel::Time_ABC& simulation,
                                  const gui::EntitySymbols& icons,
                                  DrawingsBuilder& drawingsBuilder )
    : gui::RichDockWidget( controllers, parent, objectName )
    , changeSuperiorDialog_( new gui::ChangeSuperiorDialog( controllers, icons, this ) )
    , observer_( new gui::DummyModelObserver() )
    , renameInterface_( new gui::RenameInterface( controllers, this ) )
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
    auto tactical = Configure( views_, unitTab, toolbar, tools::translate( "OrbatDockWidget", "Tactical" ), *renameInterface_,
                               new gui::RichView< TacticalTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                      "SearchTacticalTreeView",
                                                                      unitTab,
                                                                      controllers,
                                                                      filter,
                                                                      *observer_,
                                                                      icons,
                                                                      staticModel,
                                                                      simulation,
                                                                      actionsModel,
                                                                      *changeSuperiorDialog_ ) );
    connect( toolbar, SIGNAL( ChangeDisplay( int ) ), tactical->GetView(), SLOT( ChangeDisplay( int ) ) );
    // Communication
    Configure( views_, unitTab, toolbar, tools::translate( "OrbatDockWidget", "Communication" ), *renameInterface_,
               new gui::RichView< CommunicationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                           "SearchCommunicationTreeView",
                                                           unitTab,
                                                           controllers,
                                                           filter,
                                                           *observer_,
                                                           icons,
                                                           staticModel,
                                                           simulation,
                                                           actionsModel,
                                                           *changeSuperiorDialog_ ) );
    // Logistic
    auto logistic = Configure( views_, unitTab, toolbar, tools::translate( "OrbatDockWidget", "Logistic" ), *renameInterface_,
                               new gui::RichView< LogisticTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                                      "SearchLogisticTreeView",
                                                                      mainTab,
                                                                      controllers,
                                                                      filter,
                                                                      *observer_,
                                                                      icons,
                                                                      staticModel,
                                                                      simulation,
                                                                      actionsModel,
                                                                      *changeSuperiorDialog_ ) );
    logisticListView_ = logistic->GetView();
    // Object
    Configure( views_, mainTab, toolbar, tools::translate( "OrbatDockWidget", "Objects" ), *renameInterface_,
               new gui::RichView< gui::ObjectTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                         "ObjectTreeView",
                                                         mainTab,
                                                         controllers,
                                                         filter,
                                                         *observer_ ) );
    // Crowds
    Configure( views_, mainTab, toolbar, tools::translate( "OrbatDockWidget", "Crowds" ), *renameInterface_,
               new gui::RichView< PopulationTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                        "PopulationTreeView",
                                                        mainTab,
                                                        controllers,
                                                        filter,
                                                        *observer_ ) );
    // Populations
    const bool editable = true; // RichView contructor takes reference
    Configure( views_, mainTab, toolbar, tools::translate( "OrbatDockWidget", "Populations" ), *renameInterface_,
               new gui::RichView< gui::InhabitantTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
                                                             "InhabitantTreeView",
                                                             mainTab,
                                                             controllers,
                                                             filter,
                                                             *observer_,
                                                             editable ) );
    // Populations
    Configure( views_, mainTab, toolbar, tools::translate( "OrbatDockWidget", "Drawings" ), *renameInterface_,
               new gui::RichView< gui::DrawingsTreeView >( gui::RichView_ABC::eOptions_SearchLineEdit,
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
// Name: OrbatDockWidget::Load
// Created: ABR 2014-05-13
// -----------------------------------------------------------------------------
void OrbatDockWidget::Load()
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->Load();
}

// -----------------------------------------------------------------------------
// Name: OrbatDockWidget::Purge
// Created: JSR 2012-09-26
// -----------------------------------------------------------------------------
void OrbatDockWidget::Purge()
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->Purge();
    logisticListView_->Purge();
}
