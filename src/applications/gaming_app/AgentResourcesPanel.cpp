// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentResourcesPanel.h"
#include "EquipmentsListView.h"
#include "DotationsListView.h"
#include "LendingsListView.h"
#include "BorrowingsListView.h"
#include "HumansListView.h"

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : EntityPanel( parent, panel, controllers, tr( "Resources" ) )
{
    new EquipmentsListView( this, controllers, factory );
    new DotationsListView( this, controllers, factory );
    new HumansListView( this, controllers, factory );
    new LendingsListView( this, controllers, factory );
    new BorrowingsListView( this, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentResourcesPanel::~AgentResourcesPanel()
{
    // NOTHING
}
