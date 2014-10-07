// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ToolbarContainer.h"

#include "FileToolbar.h"
#include "LocationEditorToolbar.h"
#include "TerrainToolBar.h"
#include "clients_gui/GisToolbar.h"
#include "clients_gui/DisplayToolbar.h"
#include "ENT/ENT_Enums.h"
#include "preparation/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: ToolbarContainer constructor
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
ToolbarContainer::ToolbarContainer( QMainWindow* parent,
                                    kernel::Controllers& controllers,
                                    const StaticModel& staticModel,
                                    gui::View_ABC& view,
                                    const std::shared_ptr< gui::LocationsLayer >& layer,
                                    const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                                    gui::ExclusiveEventStrategy& eventStrategy,
                                    UrbanModel& urbanModel,
                                    RemoveBlocksDialog& removeBlocksDialog,
                                    gui::TerrainProfiler& terrainProfiler )
{
    // File
    {
        fileToolbar_ = new FileToolbar( parent, controllers );
        fileToolbar_->SetModes( eModes_LivingArea, eModes_Default, true );
        parent->addToolBar( fileToolbar_ );
    }
    // Display
    {
        gui::RichToolBar* displayToolBar = new gui::DisplayToolbar( parent, controllers );
        displayToolBar->SetModes( eModes_Default | eModes_LivingArea, eModes_None, true );
        parent->addToolBar( displayToolBar );
    }
    // Terrain
    {
        gui::RichToolBar* terrainToolBar = new TerrainToolBar( parent, controllers, eventStrategy, paramLayer, urbanModel, removeBlocksDialog );
        terrainToolBar->SetModes( eModes_Default | eModes_LivingArea, eModes_Terrain, true );
        parent->addToolBar( terrainToolBar );
    }
    // Location editor
    {
        locationEditorToolbar_ = new LocationEditorToolbar( parent, controllers, staticModel.coordinateConverter_, view, layer );
        locationEditorToolbar_->SetModes( eModes_Default | eModes_LivingArea, eModes_None, true );
        parent->addToolBar( locationEditorToolbar_ );
    }
    // GIS
    {
        gui::RichToolBar* gisToolbar = new gui::GisToolbar( parent, controllers, staticModel.detection_, terrainProfiler );
        gisToolbar->SetModes( eModes_Default | eModes_LivingArea, eModes_None, true );
        parent->addToolBar( gisToolbar );
    }
}

// -----------------------------------------------------------------------------
// Name: ToolbarContainer destructor
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
ToolbarContainer::~ToolbarContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToolbarContainer::GetFileToolbar
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
FileToolbar& ToolbarContainer::GetFileToolbar() const
{
    return *fileToolbar_;
}

// -----------------------------------------------------------------------------
// Name: ToolbarContainer::GetLocationEditorToolbar
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
LocationEditorToolbar& ToolbarContainer::GetLocationEditorToolbar() const
{
    return *locationEditorToolbar_;
}
