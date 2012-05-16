// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DockManager.h"
#include "TacticalListView.h"
#include "CommunicationListView.h"
#include "ObjectListView.h"
#include "PopulationListView.h"
#include "LogisticListView.h"
#include "InhabitantListView.h"
#include "UrbanListView.h"
#include "PreparationProfile.h"
#include "PropertiesPanel.h"
#include "CreationPanels.h"
#include "ExtensionsPanel.h"
#include "LivingAreaPanel.h"
#include "ResourceNetworkDialog.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"
#include "preparation/AgentsModel.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/SearchListView.h"
#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include <boost/foreach.hpp>
#include <QtGui/QToolBar.h>

// -----------------------------------------------------------------------------
// Name: DockManager constructor
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
DockManager::DockManager( QMainWindow* parent, kernel::Controllers& controllers, gui::AutomatsLayer& automats,
                          gui::FormationLayer& formation, gui::EntitySymbols& icons, ModelBuilder& modelBuilder,
                          gui::ItemFactory_ABC& factory, Model& model, StaticModel& staticModel,
                          const tools::ExerciseConfig& config, gui::SymbolIcons& symbols,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ParametersLayer& paramLayer, gui::WeatherLayer& weatherLayer,
                          gui::GlProxy& glProxy, ColorController& colorController )
    : pCreationPanel_    ( 0 )
    , pLivingAreaPanel_  ( 0 )
{
    // Agent list panel
    {
        gui::RichDockWidget* pListDockWnd = new gui::RichDockWidget( controllers, parent, "orbat", tools::translate( "DockManager", "ORBAT" ) );
        pListDockWnd->SetModes( ePreparationMode_Exercise, ePreparationMode_Default | ePreparationMode_LivingArea );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd );
        Q3VBox* box = new Q3VBox( pListDockWnd );
        pListDockWnd->setWidget( box );

        new gui::AggregateToolbar( box, controllers.controller_, automats, formation );
        gui::SearchListView_ABC* searchListView = 0;
        QTabWidget* pListsTabWidget = new QTabWidget( box );
        {
            QTabWidget* pAgentsTabWidget = new QTabWidget( pListsTabWidget );
            pListsTabWidget->addTab( pAgentsTabWidget, tools::translate( "DockManager", "Units" ) );
            // Tactical
            {
                searchListView = new gui::SearchListView< TacticalListView >( pListsTabWidget, controllers, factory, icons, modelBuilder, model.formations_.levels_, glProxy );
                listViews_.push_back( searchListView->GetRichListView() );
                pAgentsTabWidget->addTab( searchListView, tools::translate( "DockManager","Tactical" ) );
            }
            // Communication
            {
                searchListView = new gui::SearchListView< CommunicationListView >( pListsTabWidget, controllers, factory, icons, modelBuilder );
                listViews_.push_back( searchListView->GetRichListView() );
                pAgentsTabWidget->addTab( searchListView, tools::translate( "DockManager","Communication" ) );
            }
            // Logistic
            {
                searchListView = new gui::SearchListView< LogisticListView >( pListsTabWidget, controllers, factory, PreparationProfile::GetProfile(), icons, modelBuilder );
                listViews_.push_back( searchListView->GetRichListView() );
                pAgentsTabWidget->addTab( searchListView, tools::translate( "DockManager", "Logistic" ) );
            }
        }
        // Objects
        {
            searchListView = new gui::SearchListView< ObjectListView >( pListsTabWidget, controllers, factory, modelBuilder );
            listViews_.push_back( searchListView->GetRichListView() );
            pListsTabWidget->addTab( searchListView, tools::translate( "DockManager","Objects" ) );
        }
//         // Urban
//         {
//             searchListView = new gui::SearchListView< UrbanListView >( pListsTabWidget, controllers, factory, modelBuilder );
//             listViews_.push_back( searchListView->GetRichListView() );
//             pListsTabWidget->addTab( searchListView, tools::translate( "DockManager","Urban" ) );
//         }
//         // Crowds
        {
            searchListView = new gui::SearchListView< PopulationListView >( pListsTabWidget, controllers, factory, modelBuilder );
            listViews_.push_back( searchListView->GetRichListView() );
            pListsTabWidget->addTab( searchListView, tools::translate( "DockManager","Crowds" ) );
        }
        // Populations
        {
            searchListView = new gui::SearchListView< InhabitantListView >( pListsTabWidget, controllers, factory, modelBuilder );
            listViews_.push_back( searchListView->GetRichListView() );
            pListsTabWidget->addTab( searchListView, tools::translate( "DockManager","Populations" ) );
        }
    }

    // Properties panel
    {
        gui::RichDockWidget* pPropertiesDockWnd = new gui::RichDockWidget( controllers, parent, "properties", tools::translate( "DockManager", "Properties" ) );
        pPropertiesDockWnd->SetModes( ePreparationMode_Exercise | ePreparationMode_Terrain, ePreparationMode_Default | ePreparationMode_LivingArea );
        PropertiesPanel* propertiesPanel = new PropertiesPanel( pPropertiesDockWnd, controllers, model, staticModel );
        pPropertiesDockWnd->setWidget( propertiesPanel );
        parent->addDockWidget( Qt::RightDockWidgetArea, pPropertiesDockWnd );
    }
    // ResourceNetwork panel
    {
         gui::RichDockWidget* pResourceWnd = new ResourceNetworkDialog( parent, controllers, staticModel, model );
         pResourceWnd->SetModes( ePreparationMode_Terrain, ePreparationMode_Default | ePreparationMode_LivingArea );
         parent->addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
    }
    // Extensions panel
    {
        gui::RichDockWidget* pExtensionsPanel = new ExtensionsPanel( parent, controllers, staticModel.extensions_, model.agents_, model.formations_ );
        pExtensionsPanel->SetModes( ePreparationMode_None, ePreparationMode_Default | ePreparationMode_LivingArea | ePreparationMode_Terrain );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel );
    }
    // Creation panel
    {
        gui::RichDockWidget* pCreationDockWnd = new gui::RichDockWidget( controllers, parent, "creation", tools::translate( "DockManager", "Creation" ) );
        pCreationDockWnd->SetModes( ePreparationMode_None, ePreparationMode_Default | ePreparationMode_LivingArea | ePreparationMode_Terrain );
        pCreationPanel_ = new CreationPanels( pCreationDockWnd, controllers, staticModel, model, config, factory, symbols, colorStrategy, paramLayer, weatherLayer, glProxy, colorController );
        pCreationDockWnd->setWidget( pCreationPanel_ );
        parent->addDockWidget( Qt::RightDockWidgetArea, pCreationDockWnd );
    }
    // Living area panel
    {
        pLivingAreaPanel_ = new LivingAreaPanel( parent, controllers, paramLayer, glProxy );
        pLivingAreaPanel_->SetModes( ePreparationMode_LivingArea, ePreparationMode_Default | ePreparationMode_Exercise | ePreparationMode_Terrain, ePreparationMode_LivingArea );
        parent->addDockWidget( Qt::TopDockWidgetArea, pLivingAreaPanel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DockManager destructor
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
DockManager::~DockManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DockManager::Purge
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void DockManager::Purge()
{
    for( std::vector< gui::RichListView* >::iterator it = listViews_.begin(); it != listViews_.end(); ++it )
        if( *it )
            ( *it )->Purge();
}

// -----------------------------------------------------------------------------
// Name: DockManager::BlockCreationOnListViews
// Created: ABR 2012-03-29
// -----------------------------------------------------------------------------
void DockManager::BlockCreationOnListViews( bool enable )
{
    for( std::vector< gui::RichListView* >::iterator it = listViews_.begin(); it != listViews_.end(); ++it )
        if( *it )
            ( *it )->SetCreationBlocked( enable );
}

// -----------------------------------------------------------------------------
// Name: DockManager::Load
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void DockManager::Load()
{
    pCreationPanel_->Load();
}

// -----------------------------------------------------------------------------
// Name: DockManager::GetObjectCreationPanel
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
ObjectCreationPanel& DockManager::GetObjectCreationPanel() const
{
    return pCreationPanel_->GetObjectCreationPanel();
}

// -----------------------------------------------------------------------------
// Name: DockManager::GetInhabitantCreationPanel
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
InhabitantCreationPanel& DockManager::GetInhabitantCreationPanel() const
{
    return pCreationPanel_->GetInhabitantCreationPanel();
}

// -----------------------------------------------------------------------------
// Name: DockManager::GetLivingAreaPanel
// Created: LGY 2012-01-12
// -----------------------------------------------------------------------------
LivingAreaPanel& DockManager::GetLivingAreaPanel() const
{
    return *pLivingAreaPanel_;
}
