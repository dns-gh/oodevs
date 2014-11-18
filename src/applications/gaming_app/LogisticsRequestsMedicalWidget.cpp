// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsMedicalWidget.h"
#include "LogisticsRequestsDetailsTable.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"

LogisticsRequestsMedicalWidget::LogisticsRequestsMedicalWidget( QWidget* parent, kernel::Controllers& controllers,
                                                                gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                                                const SimulationController& simulationController, Model& model )
    : LogisticConsignsWidget( parent, controllers, extractor, profile, simulationController, model, eMedical )
{
    // NOTHING
}

LogisticsRequestsMedicalWidget::~LogisticsRequestsMedicalWidget()
{
    // NOTHING
}

QString LogisticsRequestsMedicalWidget::GetInjury( const LogMedicalConsign& consign )
{
    QString injury;
    if( consign.IsDiagnosed() || profile_.IsSupervision() )
    {
        injury = tools::ToString( consign.GetWound() );
        if( consign.IsMental() )
            injury.append( ", " ).append( tools::translate( "Logistic", "Psy" ) );
        if( consign.IsContamined() )
            injury.append( ", " ).append( tools::translate( "Logistic", "NBC" ) );
    }
    return injury;
}

void LogisticsRequestsMedicalWidget::OnRequestSelected( const LogisticsConsign_ABC& consign )
{
    const LogMedicalConsign& c = static_cast< const LogMedicalConsign& >( consign );
    detailsTable_->Add( tools::translate( "Logistic", "Requester:" ), GetDisplayName( c.GetConsumer() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Rank:"), tools::ToString( c.GetRank() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Injury:" ), GetInjury( c ) );
    detailsTable_->Add( tools::translate( "Logistic", "Created:" ), c.GetCreationTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Handler:" ), GetDisplayName( c.GetHandler() ) );
    detailsTable_->Add( tools::translate( "Logistic", "State:"), c.GetStatusDisplay() );
    detailsTable_->Add( tools::translate( "Logistic", "Started:" ), c.GetCurrentStartedTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Ending:" ), SupervisionFilter( c.GetCurrentEndTime() ) );
}

bool LogisticsRequestsMedicalWidget::IsActive( const LogMedicalConsign& consign ) const
{
    return consign.GetStatus() != sword::LogMedicalHandlingUpdate::finished;
}
