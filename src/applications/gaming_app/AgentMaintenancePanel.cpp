// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentMaintenancePanel.h"
#include "LogisticStatusWidgets.h"
#include "MaintenanceConsignsWidget.h"
#include "MaintenanceHaulersListView.h"
#include "MaintenanceRepairersListView.h"

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::AgentMaintenancePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : EntityPanel( parent, panel, controllers, tr( "Maintenance system" ) )
{
    new MaintenanceConsignsWidget( this, controllers, factory );
    new MaintenanceStatusWidget( this, controllers, factory );
    new MaintenanceHaulersListView( this, controllers, factory );
    new MaintenanceRepairersListView( this, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::~AgentMaintenancePanel()
{
    // NOTHING
}
