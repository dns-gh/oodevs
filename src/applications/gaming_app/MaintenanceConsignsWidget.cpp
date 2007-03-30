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

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceConsignsWidget::MaintenanceConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< MaintenanceConsignsWidget, LogMaintenanceConsign, LogMaintenanceConsigns >( parent, controllers, factory )
{
    AddConsignColumn( tr( "Equipment:" ) );
    AddConsignColumn( tr( "Breakdown:" ) );
    AddConsignColumn( tr( "State:" ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceConsignsWidget::~MaintenanceConsignsWidget()
{
    // NOTHING
}
