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
    : parent_            ( parent )
    , controllers_       ( controllers )
    , pExtensionsPanel_  ( 0 )
    , pCreationPanel_    ( 0 )
    , pLivingAreaPanel_  ( 0 )
    , geometry_          ( parent_->saveGeometry() )
    , state_             ( parent_->saveState() )
    , editionModeEnabled_( false )
{
    // Agent list panel
    {
        QDockWidget* pListDockWnd = new QDockWidget( "orbat", parent );
        pListDockWnd->setWindowTitle( tools::translate( "DockManager", "ORBAT" ) );
        pListDockWnd->setObjectName( "Orbat" );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd );
        dockWidgets_.push_back( pListDockWnd );
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
        // Crowds
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
        QDockWidget* pPropertiesDockWnd = new QDockWidget( "properties", parent );
        pPropertiesDockWnd->setObjectName( "properties" );
        parent->addDockWidget( Qt::RightDockWidgetArea, pPropertiesDockWnd );
        PropertiesPanel* propertiesPanel = new PropertiesPanel( pPropertiesDockWnd, controllers, model, staticModel );
        pPropertiesDockWnd->setWidget( propertiesPanel );
        pPropertiesDockWnd->setWindowTitle( tools::translate( "DockManager", "Properties" ) );
        dockWidgets_.push_back( pPropertiesDockWnd );
    }
    // ResourceNetwork panel
    {
         QDockWidget* pResourceWnd = new ResourceNetworkDialog( parent, controllers, staticModel, model );
         parent->addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
         pResourceWnd->hide();
         dockWidgets_.push_back( pResourceWnd );
    }
    // Extensions panel
    {
        pExtensionsPanel_ = new ExtensionsPanel( parent, controllers, staticModel.extensions_, model.agents_, model.formations_, "ExtensionsPanel" );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel_ );
        pExtensionsPanel_->hide();
        dockWidgets_.push_back( pExtensionsPanel_ );
    }
    // Creation panel
    {
        QDockWidget* pCreationDockWnd = new QDockWidget( "creation", parent );
        pCreationDockWnd->setObjectName( "creation" );
        parent->addDockWidget( Qt::RightDockWidgetArea, pCreationDockWnd );
        pCreationDockWnd->hide();
        pCreationPanel_ = new CreationPanels( pCreationDockWnd, controllers, staticModel, model, config, factory, symbols, colorStrategy, paramLayer, weatherLayer, glProxy, colorController );
        pCreationDockWnd->setWidget( pCreationPanel_ );
        pCreationDockWnd->setWindowTitle( tools::translate( "DockManager", "Creation" ) );
        dockWidgets_.push_back( pCreationDockWnd );
    }
    // Living area panel
    {
        pLivingAreaPanel_ = new LivingAreaPanel( parent, controllers, paramLayer, glProxy );
        QWidget* titleWidget = new QWidget( parent_ );
        pLivingAreaPanel_->setTitleBarWidget( titleWidget );
        pLivingAreaPanel_->titleBarWidget()->hide();
        pLivingAreaPanel_->setFeatures( QDockWidget::NoDockWidgetFeatures );
        parent->addDockWidget( Qt::TopDockWidgetArea, pLivingAreaPanel_ );
        pLivingAreaPanel_->hide();
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DockManager destructor
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
DockManager::~DockManager()
{
    pLivingAreaPanel_->hide();
    controllers_.Unregister( *this );
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
    pExtensionsPanel_->hide();
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
// Name: DockManager::Load
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void DockManager::Load()
{
    pCreationPanel_->Load();
}

// -----------------------------------------------------------------------------
// Name: DockManager::OptionChanged
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void DockManager::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "LivingAreaEditor" )
    {
        editionModeEnabled_ = value.To< bool >();
        if( editionModeEnabled_ )
        {
            geometry_ = parent_->saveGeometry();
            state_ = parent_->saveState();
        }
        ToggleWidget( dockWidgets_, widgets_ );
        QList< QWidget* > widgets;
        QList< QToolBar* > toolbars = parent_->findChildren< QToolBar* >();
        for( QList< QToolBar* >::iterator it = toolbars.begin(); it != toolbars.end(); ++it )
            widgets.push_back( *it );
        ToggleWidget( widgets, toolbars_ );
        if( parent_->menuBar() )
            parent_->menuBar()->setDisabled( editionModeEnabled_ );
        pLivingAreaPanel_->setShown( editionModeEnabled_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DockManager::ToggleWidget
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void DockManager::ToggleWidget( QList< QWidget* >& container, QList< QWidget* >& current )
{
    if( editionModeEnabled_ )
    {
        for( QList< QWidget* >::iterator it = container.begin(); it != container.end(); ++it )
            if( (*it)->isVisible() )
            {
                (*it)->setVisible( false );
                current.push_back( (*it) );
            }
    }
    else
    {
        for( QList< QWidget* >::iterator it = current.begin(); it != current.end(); ++it )
            (*it)->setVisible( true );
        current.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: DockManager::SaveGeometry
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
QByteArray DockManager::SaveGeometry() const
{
    if( editionModeEnabled_ )
        return geometry_;
    return parent_->saveGeometry();
}

// -----------------------------------------------------------------------------
// Name: DockManager::SaveState
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
QByteArray DockManager::SaveState() const
{
    if( editionModeEnabled_ )
        return state_;
    return parent_->saveState();
}

// -----------------------------------------------------------------------------
// Name: DockManager::GetLivingAreaPanel
// Created: LGY 2012-01-12
// -----------------------------------------------------------------------------
LivingAreaPanel& DockManager::GetLivingAreaPanel() const
{
    return *pLivingAreaPanel_;
}
