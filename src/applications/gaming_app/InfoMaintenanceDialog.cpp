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

#include "clients_gui/LogisticBase.h"
#include "clients_kernel/EquipmentType.h"
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
                                              const SimulationController& simulationController, Model& model )
    : InfoDialog< kernel::MaintenanceStates_ABC >( parent, controllers, "" )
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

    widget_ = new LogisticsRequestsMaintenanceWidget( tabs, controllers, extractor, profile, simulationController, model );
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
    boost::optional< bool > manual = boost::none;
    if( entity )
    {
        InfoDialog< kernel::MaintenanceStates_ABC >::NotifySelected( entity );
        if( const auto* base = entity->Retrieve< gui::LogisticBase >() )
            if( base->IsBase() )
                manual = base->IsMaintenanceManual();
    }
    UpdateTitle( manual );
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
// Name: InfoMaintenanceDialog::FillCurrentModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::FillCurrentModel( const kernel::Entity_ABC& entity )
{
    widget_->FillCurrentModel( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::FillHistoryModel
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::FillHistoryModel()
{
    widget_->FillHistoryModel();
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::UpdateTitle
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::UpdateTitle( boost::optional< bool > manual )
{
    const QString baseTitle = tools::translate( "InfoMaintenanceDialog", "Maintenance system" );
    if( manual )
        setWindowTitle( baseTitle + " - " + ( *manual
                            ? tools::translate( "InfoMaintenanceDialog", "Manual" )
                            : tools::translate( "InfoMaintenanceDialog", "Automatic" ) ) );
    else
        setWindowTitle( baseTitle );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::NotifyUpdated
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void InfoMaintenanceDialog::NotifyUpdated( const gui::LogisticBase& ext )
{
    if( selected_ && selected_->GetId() == ext.GetEntity().GetId() )
        UpdateTitle( ext.IsMaintenanceManual() );
}

// -----------------------------------------------------------------------------
// Name: InfoMaintenanceDialog::IsVisible
// Created: LGY 2014-02-05
// -----------------------------------------------------------------------------
bool InfoMaintenanceDialog::IsVisible() const
{
    return isVisible();
}
