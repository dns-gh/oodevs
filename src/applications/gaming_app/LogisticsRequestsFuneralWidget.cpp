// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsFuneralWidget.h"
#include "LogisticsRequestsDetailsTable.h"
#include "LogisticsRequestsTable.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "clients_kernel/DotationType.h"
#include <boost/bind.hpp>
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsFuneralWidget constructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsRequestsFuneralWidget::LogisticsRequestsFuneralWidget( QWidget* parent, kernel::Controllers& controllers,
                                                                gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                                                const SimulationController& simulationController, Model& model )
    : LogisticConsignsWidget( parent, controllers, extractor, profile, simulationController, model, eFuneral )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsFuneralWidget destructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsRequestsFuneralWidget::~LogisticsRequestsFuneralWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsFuneralWidget::OnRequestSelected
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsFuneralWidget::OnRequestSelected( const LogisticsConsign_ABC& consign )
{
    const LogFuneralConsign& c = static_cast< const LogFuneralConsign& >( consign );
    detailsTable_->Add( tools::translate( "Logistic", "Requester:" ),       GetDisplayName( c.GetConsumer() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Rank:" ),            tools::ToString( c.GetRank() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Current packaging:" )
        , c.GetPackagingResource()? QString::fromStdString( c.GetPackagingResource()->GetName() ) : QString() );
    detailsTable_->Add( tools::translate( "Logistic", "Created:" ), c.GetCreationTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Handler:" ),         GetDisplayName( c.GetHandler() ) );
    detailsTable_->Add( tools::translate( "Logistic", "State:" ),           c.GetStatusDisplay() );
    detailsTable_->Add( tools::translate( "Logistic", "Started:" ),         c.GetCurrentStartedTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Ending:" ),          SupervisionFilter( c.GetCurrentEndTime() ) );
}
