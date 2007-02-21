// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentSupplyPanel.h"
#include "LogisticStatusWidgets.h"
#include "SupplyConsignsWidget.h"
#include "SupplyStocksListView.h"
#include "SupplyQuotasListView.h"
#include "SupplyTransportersListView.h"

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentSupplyPanel::AgentSupplyPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : EntityPanel( parent, panel, controllers, tr( "Supply system" ) )
{
    new SupplyConsignsWidget( this, controllers, factory );
    new SupplyStatusWidget( this, controllers, factory );
    new SupplyStocksListView( this, controllers, factory );
    new SupplyQuotasListView( this, controllers, factory );
    new SupplyTransportersListView( this, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentSupplyPanel::~AgentSupplyPanel()
{
    // NOTHING
}
