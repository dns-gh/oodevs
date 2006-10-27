// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InfoPanels.h"
#include "preparation/Model.h"
#include "AgentResourcesPanel.h"
#include "PropertiesPanel.h"
#include "EditorFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* parent, Controllers& controllers, Model& model, const StaticModel& staticModel, gui::ItemFactory_ABC& factory )
    : Panels( parent )
    , editorFactory_( *new EditorFactory( controllers, model, staticModel ) )
{
    AddPanel( new PropertiesPanel( this, *this, controllers, editorFactory_ ) );
    AddPanel( new AgentResourcesPanel( this, *this, controllers, factory ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    delete &editorFactory_;
}
