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
#include "preparation/StaticModel.h"
#include "AgentStatePanel.h"
#include "AgentResourcesPanel.h"
#include "ObjectStatePanel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InfoPanels constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
InfoPanels::InfoPanels( QWidget* parent, Controllers& controllers, const StaticModel& model, ItemFactory_ABC& factory )
    : Panels( parent )
{
    AddPanel( new AgentStatePanel( this, *this, controllers ) );
    AddPanel( new AgentResourcesPanel( this, *this, controllers, factory ) );
    AddPanel( new ObjectStatePanel( this, *this, controllers ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanels destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
InfoPanels::~InfoPanels()
{
    // NOTHING
}
