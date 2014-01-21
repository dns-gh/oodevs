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
#include "gaming/Model.h"
#include "gaming/HistoryLogisticsModel.h"
#include "gaming/SimulationController.h"
#include "protocol/Protocol.h"

#include "ENT/ENT_Enums.h"

Q_DECLARE_METATYPE( const LogisticsConsign_ABC* )

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC constructor
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
LogisticConsignsWidget_ABC::LogisticConsignsWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                                        const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model,
                                                        const QStringList& requestsHeader /*= QStringList()*/ )
    : QWidget( parent )
    , controllers_( controllers )
    , extractor_( extractor )
    , historyModel_( model.historyLogistics_ )
    , profile_( profile )
    , selected_( controllers )
    , needUpdating_( true )
    , requestSelected_( 0 )
    , simulationController_( simulationController )
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
    pCheckBoxLayout->addWidget( completedCheckbox_ );

    requestsTable_ = new LogisticsRequestsTable( "Logistics requests", this, requestsHeader, model.actions_, controllers.GetCurrentMode() );
    connect( requestsTable_->selectionModel(), SIGNAL( currentRowChanged( const QModelIndex&, const QModelIndex& ) )
                                             , SLOT( OnRequestsTableSelected( const QModelIndex&, const QModelIndex& ) ) );

    detailsTable_ = new LogisticsRequestsDetailsTable( "Logistics requests details", this );
    connect( detailsTable_->GetLinkItemDelegate(), SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                                                 , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );

    historyTable_ = new LogisticsRequestsHistoryTable( "Logistics requests history", this );
    connect( historyTable_->GetLinkItemDelegate(), SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                                                 , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
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
    if( requestSelected_ )
        lastRequestId_ = requestSelected_->GetId();
    requestSelected_ = 0;
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
// Name: LogisticConsignsWidget_ABC::NotifySelected
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::SelectRequest
// Created: LGY 2013-12-11
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::SelectRequest()
{
    requestsTable_->SelectRequest( lastRequestId_ );
    requestSelected_ = requestsTable_->GetCurrentRequest();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::OnLinkClicked
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnLinkClicked( const QString& url, const QModelIndex& /*index*/ )
{
    const QString safeUrl( url );
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
    if( requestSelected_ && requestSelected_->GetId() == consign.GetId() )
        requestsTable_->SelectRequest( consign.GetId() );
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
    if( selected_ )
    {
        FillHistoryModel();
        FillCurrentModel( *selected_ );
    }
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
    kernel::Agent_ABC* consumer = consign.GetConsumer();
    kernel::Entity_ABC* handler = consign.GetHandler();
    LogisticConsignsWidget_ABC::DisplayRequest( consign, consumer ? consumer->GetName() : "",
        handler ? handler->GetName() : "" , consign.GetStatusDisplay() );
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
    const auto& states = consign.GetHistory().GetStates();
    if( !states.empty() )
    {
        historyTable_->Purge();
        bool empty = true;
        for( auto it = states.rbegin(); it != states.rend(); ++it )
            if( it->endedTick_ )
            {
                empty = false;
                historyTable_->AddRequest( consign.GetStatusDisplay( it->nStatus_ ),
                    requestSelected_->ConvertTickToTimeString( it->startedTick_ ),
                    requestSelected_->ConvertTickToTimeString( it->endedTick_ ),
                    GetDisplayName( it->handler_ ) );
            }
        if( !empty )
            historyTable_->ResizeColumnsToContents();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::IsHistoryChecked
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
bool LogisticConsignsWidget_ABC::IsHistoryChecked() const
{
    return completedCheckbox_->isChecked();
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
    simulationController_.SendHistoryRequests( requestsIds );
}
