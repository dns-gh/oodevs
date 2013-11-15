// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticConsignsWidget_ABC.h"
#include "LogisticsRequestsTable.h"
#include "LogisticsRequestsDetailsTable.h"
#include "moc_LogisticConsignsWidget_ABC.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_gui/RichCheckBox.h"
#include "protocol/ServerPublisher_ABC.h"

#include "ENT/ENT_Enums_Gen.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC constructor
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
LogisticConsignsWidget_ABC::LogisticConsignsWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor
                                                      , const QString& filter, const kernel::Profile_ABC& profile, Publisher_ABC& publisher
                                                      , const QStringList& requestsHeader /*= QStringList()*/ )
    : QWidget( parent )
    , controllers_( controllers )
    , extractor_( extractor )
    , profile_( profile )
    , publisher_( publisher )
    , selected_( controllers )
    , needUpdating_( true )
    , requestSelected_( 0 )
{
    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setSizeConstraint( QLayout::SetMaximumSize );

    QGroupBox* box = new QGroupBox( tools::translate( "LogisticConsignsWidget_ABC", "Details" ) );
    pDetailLayout_ = new QVBoxLayout( box );
    pDetailLayout_->setSizeConstraint( QLayout::SetMaximumSize );

    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout();
    pCheckBoxLayout->setAlignment( Qt::AlignRight );
    completedCheckbox_ = new gui::RichCheckBox( "completedRequestsCheckbox", tools::translate( "LogisticConsignsWidget_ABC", "Show completed requests" ) );
    completedCheckbox_->setFocusPolicy( Qt::NoFocus );
    connect( completedCheckbox_, SIGNAL( stateChanged( int ) ), SLOT( OnCompletedFilter() ) );
    // The history filtering is partially implemented but does not seem to work,
    // even if logistic requests are not destroyed. I am leaving it here, hidden,
    // until I figure out it is broken by design or not.
    completedCheckbox_->setVisible( false );
    pCheckBoxLayout->addWidget( completedCheckbox_ );

    requestsTable_ = new LogisticsRequestsTable( "Logistics requests", this, requestsHeader, filter );
    connect( requestsTable_->GetLinkItemDelegate(), SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                                                  , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
    connect( requestsTable_->selectionModel(), SIGNAL( currentRowChanged( const QModelIndex&, const QModelIndex& ) )
                                             , SLOT( OnRequestsTableSelected( const QModelIndex&, const QModelIndex& ) ) );
    requestsTable_->SetFilterActivated( !completedCheckbox_->isChecked() );

    detailsTable_ = new LogisticsRequestsDetailsTable( "Logistics requests details", this );
    connect( detailsTable_->GetLinkItemDelegate(), SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                                                 , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );

    historyTable_ = new LogisticsRequestsHistoryTable( "Logistics requests history", this );

    pLayout->addLayout( pCheckBoxLayout );
    pLayout->addWidget( requestsTable_ );
    pDetailLayout_->addWidget( detailsTable_ );
    pDetailLayout_->addWidget( historyTable_ );
    pLayout->addWidget( box );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC destructor
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
LogisticConsignsWidget_ABC::~LogisticConsignsWidget_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::Purge
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::Purge()
{
    requestsTable_->Purge();
    PurgeDetail();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::PurgeDetail
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::PurgeDetail()
{
    detailsTable_->Purge();
    historyTable_->Purge();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::showEvent
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* entity = selected_;
    NotifySelected( entity );
    QWidget::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifySelected
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::NotifySelected( const kernel::Entity_ABC* entity )
{
    requestSelected_ = 0;
    selected_ = entity;
    Purge();
    if( entity )
    {
        DisplayRequests();
        SendHistoryRequests();
        requestsTable_->SelectRequest();
        requestSelected_ = requestsTable_->GetCurrentRequest();
        requestsTable_->SetFilterActivated( !completedCheckbox_->isChecked() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifyUpdated
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::NotifyUpdated( const Simulation::sEndTick& )
{
    if( needUpdating_ )
    {
        DisplayRequests();
        SendHistoryRequests();
        needUpdating_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::OnLinkClicked
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnLinkClicked( const QString& url, const QModelIndex& /*index*/ )
{
    const QString safeUrl( url );
    Purge();
    extractor_.NotifyLinkClicked( safeUrl + "#select" );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::DisplayRequest
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::DisplayRequest( const LogisticsConsign_ABC& consign
                                               , const QString& requester, const QString& handler, const QString& state )
{
    requestsTable_->AddRequest( consign, QString::number( consign.GetId() ), requester, handler, state );
    if( requestSelected_ && requestSelected_->GetId() ==  consign.GetId() )
        requestsTable_->SelectRequest( requestSelected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::DisplayHistory
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::DisplayHistory( const LogisticsConsign_ABC::History& history )
{
    if( requestSelected_ && requestSelected_->GetId() == history.GetConsign().GetId() )
        DisplayCurrentHistory();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::OnRequestsTableSelected
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnRequestsTableSelected( const QModelIndex& current, const QModelIndex& /*previous*/ )
{
    if( !current.isValid() )
        return;
    PurgeDetail();
    const LogisticsConsign_ABC* pConsign = requestsTable_->GetRequest( current );
    if( pConsign )
    {
        requestSelected_ = pConsign;
        OnRequestSelected( *pConsign );
        DisplayCurrentHistory();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::OnCompletedFilter
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnCompletedFilter()
{
    requestsTable_->SetFilterActivated( !completedCheckbox_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::GetDisplayName
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticConsignsWidget_ABC::GetDisplayName( const kernel::Entity_ABC* pEntity ) const
{
    if( !pEntity )
        return QString();
    if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( pEntity ) )
        return extractor_.GetDisplayName( *pAgent );
    if( const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( pEntity ) )
        return extractor_.GetDisplayName( *pAutomat );
    if( const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( pEntity ) )
        return extractor_.GetDisplayName( *pFormation );
    return extractor_.GetDisplayName( *pEntity );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::SupervisionFilter
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticConsignsWidget_ABC::SupervisionFilter( const QString& value ) const
{
    if( profile_.IsSupervision() )
        return value;
    return tools::translate( "LogisticConsignsWidget_ABC", "Unknown" );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::DisplayRequest
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::DisplayRequest( const LogisticsConsign_ABC& consign )
{
    LogisticConsignsWidget_ABC::DisplayRequest( consign, GetDisplayName( consign.GetConsumer() ),
        GetDisplayName( consign.GetHandler() ), consign.GetStatusDisplay() );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::DisplayCurrentHistory
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::DisplayCurrentHistory()
{
    if( requestSelected_ )
        DisplayRequestHistory( *requestSelected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::DisplayRequestHistory
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::DisplayRequestHistory( const LogisticsConsign_ABC& consign )
{
    historyTable_->Purge();
    const auto& states = consign.GetHistory().GetStates();
    for( auto it = states.rbegin(); it != states.rend(); ++it )
        if( it->endedTick_ )
            historyTable_->AddRequest( consign.GetStatusDisplay( it->nStatus_ ),
                requestSelected_->ConvertTimeToString( it->startedTick_ ),
                requestSelected_->ConvertTimeToString( it->endedTick_ ),
                GetDisplayName( it->handler_ ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::SendHistoryRequest
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::SendHistoryRequest( const LogisticsConsign_ABC& consign )
{
    sword::ClientToSim msg;
    msg.mutable_message()->mutable_logistic_history_request()
        ->add_requests()->set_id( consign.GetId() );
    publisher_.Send( msg );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::SendHistoryRequests
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::SendHistoryRequests()
{
    std::set< unsigned int > requestsIds;
    requestsTable_->FindRequestsIds( requestsIds );
    if( requestsIds.empty() )
        return;
    sword::ClientToSim msg;
    auto logRequest = msg.mutable_message()->mutable_logistic_history_request();
    for( auto it = requestsIds.begin(); it != requestsIds.end(); ++it )
        logRequest->add_requests()->set_id( *it );
    publisher_.Send( msg );
}
