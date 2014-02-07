// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticsRequestsSupplyWidget.h"
#include "moc_LogisticsRequestsSupplyWidget.cpp"
#include "LogisticsRequestsDetailsTable.h"
#include "LogisticsRequestsSupplyTable.h"
#include "LogisticsRequestsTable.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LinkItemDelegate.h"
#include "gaming/SupplyRecipientResourcesRequest.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/SupplyRecipientResourcesRequest.h"
#include "gaming/SupplyResourceRequest.h"
#include <boost/bind.hpp>
#include "protocol/ServerPublisher_ABC.h"

namespace
{
    QStringList GetRequestsHeader()
    {
        QStringList header;
        header << tools::translate( "LogisticsRequestsSupplyWidget", "Id" )
                << tools::translate( "LogisticsRequestsSupplyWidget", "Recipient(s)" )
                << tools::translate( "LogisticsRequestsSupplyWidget", "Supplier" )
                << tools::translate( "LogisticsRequestsSupplyWidget", "State" );
        return header;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget constructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsRequestsSupplyWidget::LogisticsRequestsSupplyWidget( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                                              const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model )
    : LogisticConsignsWidget( parent, controllers, extractor, profile, simulationController, model, eSupply, GetRequestsHeader() )
{
    supplyTable_ = new LogisticsRequestsSupplyTable( "Logistics requests supply details", this );
    connect( supplyTable_->GetLinkItemDelegate(), SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                                                , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
    pDetailLayout_->insertWidget( 1, supplyTable_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget destructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsRequestsSupplyWidget::~LogisticsRequestsSupplyWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::Purge
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyWidget::Purge()
{
    supplyTable_->Purge();
    LogisticConsignsWidget_ABC::Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::PurgeDetail
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyWidget::PurgeDetail()
{
    supplyTable_->Purge();
    LogisticConsignsWidget_ABC::PurgeDetail();
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::GetRecipientsLinks
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticsRequestsSupplyWidget::GetRecipientsLinks( const LogSupplyConsign& consign, bool link )
{
    QString recipients;
    tools::Iterator< const SupplyRecipientResourcesRequest& > itRecipient = consign.CreateIterator();
    while( itRecipient.HasMoreElements() )
    {
        const SupplyRecipientResourcesRequest& curRecipient= itRecipient.NextElement();
        if( !recipients.isEmpty() )
            recipients.append( ", ");
        recipients += link ? GetDisplayName( &curRecipient.recipient_ ) : curRecipient.recipient_.GetName();
    }
    return recipients;
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::DisplayRequest
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyWidget::DisplayRequest( const LogisticsConsign_ABC& consign )
{
    const LogSupplyConsign& c = static_cast< const LogSupplyConsign& >( consign );
    kernel::Entity_ABC* handler = c.GetHandler();
    LogisticConsignsWidget_ABC::DisplayRequest( c, GetRecipientsLinks( c, false ), handler ? handler->GetName() : "", c.GetStatusDisplay() );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::OnRequestSelected
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyWidget::OnRequestSelected( const LogisticsConsign_ABC& consign )
{
    const LogSupplyConsign& c = static_cast< const LogSupplyConsign& >( consign );
    detailsTable_->Add( tools::translate( "Logistic", "Recipient(s):" ),        GetRecipientsLinks( c, true ) );
    detailsTable_->Add( tools::translate( "Logistic", "Transport provider:"),   GetDisplayName( c.GetProviding() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Conveyor:" ),            GetDisplayName( c.GetConvoy() ) );
    detailsTable_->Add( tools::translate( "Logistic", "Request date:" ),        c.GetCreationTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Supplier:" ),            GetDisplayName( c.GetHandler() ) );
    detailsTable_->Add( tools::translate( "Logistic", "State:"),                c.GetStatusDisplay() );
    detailsTable_->Add( tools::translate( "Logistic", "Started:" ),             c.GetCurrentStartedTime() );
    detailsTable_->Add( tools::translate( "Logistic", "Ending:" ),              SupervisionFilter( c.GetCurrentEndTime() ) );
    FillSupplyTable( c );
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::DisplayHistory
// Created: LGY 2014-01-28
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyWidget::DisplayHistory( const LogSupplyConsign::History& history )
{
    const LogSupplyConsign& c = static_cast< const LogSupplyConsign& >( history.GetConsign() );
    // Update requests table
    requestsTable_->SetData( 1, GetRecipientsLinks( c, false ), history.GetConsign() );
    if( requestSelected_ && requestSelected_->GetId() == history.GetConsign().GetId() )
    {
        // Update details table
        detailsTable_->Set( tools::translate( "Logistic", "Recipient(s):" ), GetRecipientsLinks( c, true ) );
        detailsTable_->Set( tools::translate( "Logistic", "Started:" ), requestSelected_->GetCurrentStartedTime() );
        // Update supply table
        FillSupplyTable( c );
        DisplayCurrentHistory();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticsRequestsSupplyWidget::FillSupplyTable
// Created: LGY 2014-01-28
// -----------------------------------------------------------------------------
void LogisticsRequestsSupplyWidget::FillSupplyTable( const LogSupplyConsign& consign )
{
    supplyTable_->Purge();
    tools::Iterator< const SupplyRecipientResourcesRequest& > itRecipient = consign.CreateIterator();
    while( itRecipient.HasMoreElements() )
    {
        const SupplyRecipientResourcesRequest& curRecipient = itRecipient.NextElement();
        tools::Iterator< const SupplyResourceRequest& > itRequest = curRecipient.CreateIterator();
        while( itRequest.HasMoreElements() )
        {
            const SupplyResourceRequest& curRequest = itRequest.NextElement();
            supplyTable_->AddRecipientResource( GetDisplayName( &curRecipient.recipient_ ), curRequest.GetTypeName(),
                                            curRequest.GetRequested(), curRequest.GetGranted(),
                                            curRequest.GetConvoyed(), curRequest.IsDelivered() );
        }
    }
    supplyTable_->ResizeColumnsToContents();
}
