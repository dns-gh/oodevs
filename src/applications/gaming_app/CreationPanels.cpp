// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CreationPanels.h"
#include "preparation/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_gui/UnitsPanel.h"
#include "ObjectCreationPanel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CreationPanels constructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
CreationPanels::CreationPanels( QWidget* parent, Controllers& controllers, const StaticModel& staticModel, ItemFactory_ABC& factory, Publisher_ABC& publisher, ParametersLayer& paramLayer, GlTools_ABC& tools )
    : Panels( parent )
{
    AddPanel( new gui::UnitsPanel ( this, *this, controllers, staticModel.types_, factory ) );
    objectCreationPanel_ = new ObjectCreationPanel( this, *this, controllers, publisher, staticModel, paramLayer, tools );
    AddPanel( objectCreationPanel_ );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels destructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
CreationPanels::~CreationPanels()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreationPanels::Draw
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void CreationPanels::Draw( Viewport_ABC& viewport ) const
{
    objectCreationPanel_->Draw( viewport );
}
