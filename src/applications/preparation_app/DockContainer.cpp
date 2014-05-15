// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DockContainer.h"
#include "PropertiesPanel.h"
#include "CreationPanels.h"
#include "LivingAreaPanel.h"
#include "OrbatDockWidget.h"
#include "ResourceNetworkDialog.h"
#include "UrbanInfosDockWidget.h"
#include "UsagesDockWidget.h"
#include "preparation/StaticModel.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/Model.h"
#include "clients_gui/ExtensionsPanel.h"
#include "clients_gui/GlProxy.h"
#include "clients_gui/RichDockWidget.h"
#include "clients_gui/RichTreeView.h"
#include "clients_gui/SearchTreeView.h"
#include "clients_gui/TerrainProfiler.h"
#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: DockContainer constructor
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
DockContainer::DockContainer( QMainWindow* parent, kernel::Controllers& controllers, gui::AutomatsLayer& automats,
                              gui::FormationLayer& formation, gui::EntitySymbols& icons, ModelBuilder& modelBuilder,
                              Model& model, StaticModel& staticModel, const tools::ExerciseConfig& config, gui::SymbolIcons& symbols,
                              gui::ColorStrategy_ABC& colorStrategy, gui::ParametersLayer& paramLayer, gui::WeatherLayer& weatherLayer,
                              gui::GlProxy& glProxy, ColorController& colorController, gui::TerrainProfilerLayer& terrainProfileLayer,
                              const kernel::Profile_ABC& profile )
    : pCreationPanel_  ( 0 )
    , pLivingAreaPanel_( 0 )
{
    gui::SubObjectName subObject( "DockContainer" );
    // Agent list panel
    {
        gui::RichDockWidget* pListDockWnd = new OrbatDockWidget( controllers, parent, "orbat", tools::translate( "DockContainer", "ORBAT" ),
                                                                 automats, formation, icons, modelBuilder, model, staticModel, treeViews_, symbols,
                                                                 paramLayer );
        pListDockWnd->SetModes( eModes_Default | eModes_LivingArea, eModes_None, true );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pListDockWnd );
    }

    // Properties panel
    {
        gui::RichDockWidget* pPropertiesDockWnd = new gui::RichDockWidget( controllers, parent, "properties", tools::translate( "DockContainer", "Properties" ) );
        pPropertiesDockWnd->SetModes( eModes_Default | eModes_LivingArea, eModes_None, true );
        PropertiesPanel* propertiesPanel = new PropertiesPanel( pPropertiesDockWnd, controllers, model, staticModel, glProxy, config );
        pPropertiesDockWnd->setWidget( propertiesPanel );
        parent->addDockWidget( Qt::RightDockWidgetArea, pPropertiesDockWnd );
    }
    // Creation panel
    {
        gui::RichDockWidget* pCreationDockWnd = new gui::RichDockWidget( controllers, parent, "creation", tools::translate( "DockContainer", "Creation" ) );
        pCreationDockWnd->SetModes( eModes_Default | eModes_LivingArea | eModes_Terrain );
        pCreationPanel_ = new CreationPanels( pCreationDockWnd, controllers, staticModel, model, config, symbols, colorStrategy, paramLayer, weatherLayer, glProxy, colorController );
        pCreationDockWnd->setWidget( pCreationPanel_ );
        parent->addDockWidget( Qt::RightDockWidgetArea, pCreationDockWnd );
    }
    // Terrain profile
    {
        terrainProfiler_ = new gui::TerrainProfiler( parent, controllers, staticModel.detection_, terrainProfileLayer );
        terrainProfiler_->SetModes( eModes_Default | eModes_LivingArea );
        terrainProfiler_->hide();
        parent->addDockWidget( Qt::RightDockWidgetArea, terrainProfiler_ );
    }
    // Extensions panel
    {
        gui::RichDockWidget* pExtensionsPanel = new gui::ExtensionsPanel( parent, controllers, staticModel.extensions_, *model.agents_, *model.formations_, profile );
        pExtensionsPanel->SetModes( eModes_Default | eModes_LivingArea | eModes_Terrain );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pExtensionsPanel );
    }
    // Urban informations
    {
        gui::RichDockWidget* pInformations = new UrbanInfosDockWidget( parent, controllers, *model.urban_ );
        pInformations->SetModes( eModes_Default | eModes_LivingArea );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pInformations );
    }
    // Usages
    {
        pUsagesPanel_ = new UsagesDockWidget( parent, controllers, staticModel );
        pUsagesPanel_->SetModes( eModes_Default | eModes_LivingArea | eModes_Prepare );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pUsagesPanel_ );
    }
    // ResourceNetwork panel
    {
        gui::RichDockWidget* pResourceWnd = new ResourceNetworkDialog( parent, model, controllers, staticModel, model );
        pResourceWnd->SetModes( eModes_Default | eModes_LivingArea );
        parent->addDockWidget( Qt::LeftDockWidgetArea, pResourceWnd );
    }
    // Living area panel
    {
        pLivingAreaPanel_ = new LivingAreaPanel( parent, controllers, paramLayer, glProxy );
        pLivingAreaPanel_->SetModes( eModes_Default | eModes_Prepare | eModes_Terrain, eModes_LivingArea );
        parent->addDockWidget( Qt::TopDockWidgetArea, pLivingAreaPanel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DockContainer destructor
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
DockContainer::~DockContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DockContainer::Purge
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void DockContainer::Purge()
{
    for( std::vector< gui::SearchTreeView_ABC* >::iterator it = treeViews_.begin(); it != treeViews_.end(); ++it )
        if( *it )
            ( *it )->Purge();
    pCreationPanel_->Purge();
}

// -----------------------------------------------------------------------------
// Name: DockContainer::BlockCreationOnListViews
// Created: ABR 2012-03-29
// -----------------------------------------------------------------------------
void DockContainer::BlockCreationOnListViews( bool enable )
{
    for( std::vector< gui::SearchTreeView_ABC* >::iterator it = treeViews_.begin(); it != treeViews_.end(); ++it )
        if( *it && ( *it )->GetRichTreeView() )
            ( *it )->GetRichTreeView()->SetCreationBlocked( enable );
}

// -----------------------------------------------------------------------------
// Name: DockContainer::Load
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void DockContainer::Load()
{
    for( std::vector< gui::SearchTreeView_ABC* >::iterator it = treeViews_.begin(); it != treeViews_.end(); ++it )
        if( *it )
            ( *it )->Load();
    pCreationPanel_->Load();
    pUsagesPanel_->Initialize();
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetObjectCreationPanel
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
ObjectCreationPanel& DockContainer::GetObjectCreationPanel() const
{
    return pCreationPanel_->GetObjectCreationPanel();
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetInhabitantCreationPanel
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
InhabitantCreationPanel& DockContainer::GetInhabitantCreationPanel() const
{
    return pCreationPanel_->GetInhabitantCreationPanel();
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetLivingAreaPanel
// Created: LGY 2012-01-12
// -----------------------------------------------------------------------------
LivingAreaPanel& DockContainer::GetLivingAreaPanel() const
{
    return *pLivingAreaPanel_;
}

// -----------------------------------------------------------------------------
// Name: DockContainer::GetTerrainProfiler
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
gui::TerrainProfiler& DockContainer::GetTerrainProfiler() const
{
    return *terrainProfiler_;
}
