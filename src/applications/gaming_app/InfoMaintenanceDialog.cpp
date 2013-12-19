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
#include "LogisticsRequestsMaintenanceWidget.h"
#include "LogisticStatusWidgets.h"
#include "MaintenanceHaulersListView.h"
#include "MaintenanceRepairersListView.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/LogMaintenanceConsign.h"

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMaintenanceDialog::InfoMaintenanceDialog( QWidget* parent, kernel::Controllers& controllers,
                                              gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                              Publisher_ABC& publisher, Model& model )
    : InfoDialog< kernel::MaintenanceStates_ABC >( parent, controllers, tools::translate( "InfoMaintenanceDialog", "Maintenance system" ) )
    , widget_( 0 )
{
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    QWidget* pHaulersRepairersWidget = new QWidget( tabs );
    QVBoxLayout* pHaulersRepairersLayout = new QVBoxLayout();
    pHaulersRepairersLayout->addWidget( new MaintenanceHaulersListView( tabs, controllers ) );
    pHaulersRepairersLayout->addWidget( new MaintenanceRepairersListView( tabs, controllers ) );
    pHaulersRepairersWidget->setLayout( pHaulersRepairersLayout );

    QWidget* statusWidget = new QWidget();
    QVBoxLayout* statusLayout = new QVBoxLayout( statusWidget );
    statusLayout->addWidget( new MaintenanceStatusWidget( tabs, controllers ) );

    widget_ = new LogisticsRequestsMaintenanceWidget( tabs, controllers, extractor, profile, publisher, model );
    tabs->addTab( widget_, tools::translate( "InfoMaintenanceDialog", "Instructions" ) );
    tabs->addTab( pHaulersRepairersWidget, tools::translate( "InfoMaintenanceDialog", "Equipment availabilities" ) );
    tabs->addTab( statusWidget, tools::translate( "InfoMaintenanceDialog", "Chain status" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoMaintenanceDialog::~InfoMaintenanceDialog()
{
    // NOTHING
}

namespace
{
    struct MaintenanceRelevant
    {
        MaintenanceRelevant() {}
        bool operator()( const kernel::Entity_ABC& element )
        {
            const LogMaintenanceConsigns* consigns = element.Retrieve< LogMaintenanceConsigns >();
            return ( ( consigns && consigns->IsRelevant() ) || element.Retrieve< kernel::MaintenanceStates_ABC >() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoMaintenanceDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    MaintenanceRelevant maintenanceRelevant;
    return logistic_helpers::CheckEntityAndSubordinatesUpToBaseLog( element, maintenanceRelevant );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::NotifySelected
// Created: MMC 2013-01-09
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( entity )
        InfoDialog< kernel::MaintenanceStates_ABC >::NotifySelected( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::NotifyUpdated
// Created: NPT 2013-05-16
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::NotifyUpdated( const Equipments& /*equipement*/ )
{
    if( selected_ )
        InfoDialog< kernel::MaintenanceStates_ABC >::NotifyUpdated( selected_ );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::Purge
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::Purge()
{
    widget_->Purge();
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::Fill
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::Fill( const kernel::Entity_ABC& entity )
{
    widget_->Fill( entity );
}
