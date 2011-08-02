// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoMaintenanceDialog.h"
#include "LogisticStatusWidgets.h"
#include "MaintenanceConsignsWidget.h"
#include "MaintenanceHaulersListView.h"
#include "MaintenanceRepairersListView.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMaintenanceDialog::InfoMaintenanceDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : InfoDialog< MaintenanceStates >( parent, controllers, tools::translate( "InfoMaintenanceDialog", "Maintenance system" ) )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    tabs->addTab( new MaintenanceConsignsWidget( tabs, controllers, factory ), tools::translate( "InfoMaintenanceDialog", "Consigns" ) );
    Q3VBox* box = new Q3VBox( tabs );
    new MaintenanceHaulersListView( box, controllers, factory );
    new MaintenanceRepairersListView( box, controllers, factory );
    tabs->addTab( box, tools::translate( "InfoMaintenanceDialog", "Equipment availabilities" ) );
    new MaintenanceStatusWidget( RootWidget(), controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMaintenanceDialog::~InfoMaintenanceDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoMaintenanceDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    const LogMaintenanceConsigns* consigns = element.Retrieve< LogMaintenanceConsigns >();
    return ( consigns && consigns->IsRelevant() ) || element.Retrieve< MaintenanceStates >();
}
