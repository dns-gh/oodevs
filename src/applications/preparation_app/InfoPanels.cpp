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
#include "AgentStatePanel.h"
#include "AgentResourcesPanel.h"
#include "AutomatStatePanel.h"
#include "ObjectStatePanel.h"
#include "EditorFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* parent, Controllers& controllers, Model& model, gui::ItemFactory_ABC& factory )
    : Panels( parent )
    , editorFactory_( *new EditorFactory( controllers, model ) )
{
    AddPanel( new AgentStatePanel( this, *this, controllers, editorFactory_ ) );
    AddPanel( new AgentResourcesPanel( this, *this, controllers, factory ) );
    AddPanel( new AutomatStatePanel( this, *this, controllers, editorFactory_ ) );
    AddPanel( new ObjectStatePanel( this, *this, controllers, editorFactory_ ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    delete &editorFactory_;
}
