// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticMaintenanceSelectionDialog.h"
#include "LogisticsRequestsMaintenanceWidget.h"
#include "LogisticsRequestsDetailsTable.h"
#include "LogisticsRequestsTable.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/Model.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Profile_ABC.h"
#include <boost/bind.hpp>
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsMaintenanceWidget constructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsRequestsMaintenanceWidget::LogisticsRequestsMaintenanceWidget( QWidget* parent, kernel::Controllers& controllers,
                                                                        gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                                                        const SimulationController& simulationController, Model& model )
    : LogisticConsignsWidget( parent, controllers, extractor, profile, simulationController, model, eMaintenance )
{
    selectionDialog_.reset( new LogisticMaintenanceSelectionDialog( "maintenance-selection-dialog", this, controllers, model.actions_ ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsMaintenanceWidget destructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsRequestsMaintenanceWidget::~LogisticsRequestsMaintenanceWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsMaintenanceWidget::GetBreakdown
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticsRequestsMaintenanceWidget::GetBreakdown( const LogMaintenanceConsign& consign )
{
    if( consign.IsDiagnosed() || profile_.IsSupervision() )
        return QString::fromStdString( consign.GetBreakdown()->GetName() );
    return QString::fromStdString( consign.GetBreakdown()->GetUnknownName() );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsMaintenanceWidget::OnRequestSelected
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsMaintenanceWidget::OnRequestSelected( const LogisticsConsign_ABC& consign )
{
    const LogMaintenanceConsign& c = static_cast< const LogMaintenanceConsign& >( consign );
    detailsTable_->Add( tools::translate( "Logistic", "Requester:" ),       GetDisplayName( c.GetConsumer() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Equipment:"),        QString::fromStdString( c.GetEquipment()->GetName() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Breakdown:" ),       GetBreakdown( c ) );
    detailsTable_->Add( tools::translate( "Logistic", "Created:" ), c.GetCreationTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Handler:" ),         GetDisplayName( c.GetHandler() ) );
    detailsTable_->Add( tools::translate( "Logistic", "State:"),            c.GetStatusDisplay() );
    detailsTable_->Add( tools::translate( "Logistic", "Started:" ),         c.GetCurrentStartedTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Ending:" ),          SupervisionFilter( c.GetCurrentEndTime() ) );
}
