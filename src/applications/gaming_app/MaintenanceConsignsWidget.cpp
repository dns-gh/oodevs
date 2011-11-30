// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MaintenanceConsignsWidget.h"
#include "gaming/LogMaintenanceConsign.h"
#include "gaming/MaintenanceStates.h"
#include "clients_kernel/tools.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceConsignsWidget::MaintenanceConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< MaintenanceConsignsWidget, LogMaintenanceConsign, LogMaintenanceConsigns >( parent, controllers, factory )
{
    AddConsignColumn( tools::translate( "LogisticConsignsWidget_ABC", "Consumer:" ) );
    AddConsignColumn( tools::translate( "LogisticConsignsWidget_ABC", "Handler:" ) );
    AddConsignColumn( tools::translate( "MaintenanceConsignsWidget", "Equipment:" ) );
    AddConsignColumn( tools::translate( "MaintenanceConsignsWidget", "Breakdown:" ) );
    AddConsignColumn( tools::translate( "MaintenanceConsignsWidget", "State:" ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceConsignsWidget::~MaintenanceConsignsWidget()
{
    // NOTHING
}
