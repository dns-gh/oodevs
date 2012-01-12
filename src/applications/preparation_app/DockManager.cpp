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
#include "LivingAreaPanel.h"
#include "ResourceNetworkDialog.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/FormationModel.h"
#include "preparation/AgentsModel.h"
#include "clients_gui/AggregateToolbar.h"
#include "clients_gui/EntitySearchBox.h"
#include "clients_gui/LogisticList.h"
#include "clients_gui/ExtensionsPanel.h"
#include "clients_gui/GlProxy.h"
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
                          gui::GlProxy& glProxy )
    : parent_            ( parent )
    , controllers_       ( controllers )
    , logisticListView_  ( 0 )
    , pExtensionsPanel_  ( 0 )
    , pCreationPanel_    ( 0 )
    , pLivingAreaPanel_  ( 0 )
    , geometry_          ( parent_->saveGeometry() )
    , state_             ( parent_->saveState() )
    , editionModeEnabled_( false )
{
    // Agent list panel
    QDockWidget* pListDockWnd = new QDockWidget( "orbat", parent );
    pListDockWnd->setObjectName( "Orbat" );
    parent->addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd );
    dockWidgets_.push_back( pListDockWnd );
    Q3VBox* box = new Q3VBox( pListDockWnd );
    new gui::AggregateToolbar( box, controllers.controller_, automats, formation );

    QTabWidget* pListsTabWidget = new QTabWidget( box );
    {
        QTabWidget* pAgentsTabWidget = new QTabWidget( pListsTabWidget );
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new gui::EntitySearchBox< kernel::Agent_ABC >( listsTabBox, controllers );
        new TacticalListView( listsTabBox, controllers, factory, icons, modelBuilder, model.formations_.levels_ );
        pAgentsTabWidget->addTab( listsTabBox, tools::translate( "DockManager","Tactical" ) );

        listsTabBox = new Q3VBox( pListsTabWidget );
        new gui::EntitySearchBox< kernel::Agent_ABC >( listsTabBox, controllers );
        new CommunicationListView( listsTabBox, controllers, factory, icons, modelBuilder );
        pAgentsTabWidget->addTab( listsTabBox, tools::translate( "DockManager", "Communication" ) );

        listsTabBox = new Q3VBox( pListsTabWidget );
        logisticListView_ = new gui::LogisticList< LogisticListView >( controllers, factory, PreparationProfile::GetProfile(), icons, modelBuilder );
        pAgentsTabWidget->addTab( logisticListView_, tools::translate( "DockManager", "Logistic" ) );
        pListsTabWidget->addTab( pAgentsTabWidget, tools::translate( "DockManager", "Units" ) );
    }
    {
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new gui::EntitySearchBox< kernel::Object_ABC >( listsTabBox, controllers );
        new ObjectListView( listsTabBox, controllers, factory, modelBuilder );
        pListsTabWidget->addTab( listsTabBox, tools::translate( "DockManager", "Objects" ) );
    }
    {
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new gui::EntitySearchBox< kernel::Population_ABC >( listsTabBox, controllers );
        new PopulationListView( listsTabBox, controllers, factory, modelBuilder );
        pListsTabWidget->addTab( listsTabBox, tools::translate( "DockManager", "Crowds" ) );
    }
    {
        Q3VBox* listsTabBox = new Q3VBox( pListsTabWidget );
        new gui::EntitySearchBox< kernel::Inhabitant_ABC >( listsTabBox, controllers );
        new InhabitantListView( listsTabBox, controllers, factory, modelBuilder );
        pListsTabWidget->addTab( listsTabBox, tools::translate( "DockManager", "Populations" ) );
    }
    pListDockWnd->setWindowTitle( tools::translate( "DockManager", "ORBAT" ) );
    pListDockWnd->setWidget( box );
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
        pExtensionsPanel_ = new gui::ExtensionsPanel( parent, controllers, staticModel.extensions_, model.agents_, factory, icons, PreparationProfile::GetProfile(), "ExtensionsPanel" );
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
        pCreationPanel_ = new CreationPanels( pCreationDockWnd, controllers, staticModel, model, config, factory, symbols, colorStrategy, paramLayer, weatherLayer, glProxy );
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
    if( logisticListView_ )
        logisticListView_->Purge();
    pExtensionsPanel_->hide();
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
