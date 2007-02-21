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

// -----------------------------------------------------------------------------
// Name: MaintenanceStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceStatusWidget::MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticStatusWidget_ABC< MaintenanceStates >( parent, controllers, factory )
{
    display_->AddGroup( tr( "Maintenance system state" ) )
                .AddLabel( tr( "System status" ) )
                .AddLabel( tr( "Working scheme" ) )
                .AddLabel( tr( "Priorities" ) )
                .AddLabel( tr( "Tactical priorities" ) );
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
    display_->AddGroup( tr( "Medical system state" ) )
                .AddLabel( tr( "System status" ) )
                .AddLabel( tr( "Priorities" ) )
                .AddLabel( tr( "Tactical priorities" ) );
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
    display_->AddGroup( tr( "Supply system state" ) )
                .AddLabel( tr( "System status" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyStatusWidget::~SupplyStatusWidget()
{
    // NOTHING
}