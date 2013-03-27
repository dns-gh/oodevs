// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticStatusWidgets.h"
#include "clients_kernel/tools.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceStatusWidget::MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticStatusWidget_ABC< kernel::MaintenanceStates_ABC >( parent, controllers, factory )
{
    display_->AddGroup( "" )
                .AddLabel( "SystemStatus", tools::findTranslation( "MaintenanceStates", "System status" ) )
                .AddLabel( "Working", tools::findTranslation( "MaintenanceStates", "Working scheme" ) )
                .AddLabel( "Priorities", tools::findTranslation( "MaintenanceStates", "Priorities" ) )
                .AddLabel( "Tactical", tools::findTranslation( "MaintenanceStates", "Tactical priorities" ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceStatusWidget::~MaintenanceStatusWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalStatusWidget::MedicalStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticStatusWidget_ABC< MedicalStates >( parent, controllers, factory )
{
    display_->AddGroup( "" )
                .AddLabel( "Priorities", tools::findTranslation( "MedicalStates", "Priorities" ) )
                .AddLabel( "Tactical", tools::findTranslation( "MedicalStates", "Tactical priorities" ) )
                .AddLabel( "Systemstatus", tools::findTranslation( "MedicalStates", "System status" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalStatusWidget::~MedicalStatusWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyStatusWidget::SupplyStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticStatusWidget_ABC< SupplyStates >( parent, controllers, factory )
{
    display_->AddGroup( "" )
                .AddLabel( "SystemStatus", tools::findTranslation( "SupplyStates", "System status" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyStatusWidget::~SupplyStatusWidget()
{
    // NOTHING
}