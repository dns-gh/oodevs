// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticsConsign_ABC.h"
#include "clients_kernel/Tools.h"
#include "Simulation.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC constructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsConsign_ABC::LogisticsConsign_ABC( unsigned int nID,  kernel::Controller& controller, const Simulation& simulation, unsigned int creationTick )
    : controller_         ( controller )
    , simulation_         ( simulation )
    , nID_                ( nID )
    , currentStateEndTick_( std::numeric_limits< unsigned int >::max() )
    , creationTick_       ( creationTick )
    , history_           ( new History( *this ) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC destructor
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
LogisticsConsign_ABC::~LogisticsConsign_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC GetId
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
unsigned int LogisticsConsign_ABC::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC ConvertTimeToString
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticsConsign_ABC::ConvertTickToTimeString( unsigned int tick ) const
{
    if( tick == std::numeric_limits< unsigned int >::max() || tick == 0 )
        return tools::translate( "Logistic", "Unknown" );
    QString currentEndTime;
    const QDateTime endDate = simulation_.GetInitialDateTime().addSecs(
            tick * simulation_.GetTickDuration() );
    if( endDate.date() != simulation_.GetDateTime().date() )
        currentEndTime += endDate.date().toString() + " ";
    currentEndTime += endDate.time().toString();
    return currentEndTime;
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC GetCreationTime
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticsConsign_ABC::GetCreationTime() const
{
    return ConvertTickToTimeString( creationTick_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC GetCurrentEndTime
// Created: MMC 2013-09-16
// -----------------------------------------------------------------------------
QString LogisticsConsign_ABC::GetCurrentEndTime() const
{
    return ConvertTickToTimeString( currentStateEndTick_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC::GetHistory
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
const LogisticsConsign_ABC::History& LogisticsConsign_ABC::GetHistory() const
{
    return *history_;
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC::UpdateHistory
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
void LogisticsConsign_ABC::UpdateHistory( int start, int end,
        const google::protobuf::RepeatedPtrField< sword::LogisticHistoryState >& history )
{
    history_->Clear();
    for( int i = start; i != end; ++i )
    {
        const auto& msg = history.Get( i );
        if( !msg.status() )
            continue;
        HistoryState state;
        state.nStatus_ = msg.status();
        if( msg.has_handler() )
            state.handler_ = GetRequestHandler( msg.handler().id() );
        state.startedTick_  = msg.start_tick();
        state.endedTick_    = msg.end_tick();
        history_->Add( state );
    }
    controller_.Update( *history_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC::GetStatusLastStarted
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
QString LogisticsConsign_ABC::GetStatusLastStarted( int status ) const
{
    unsigned int tickStarted = 0;
    for( auto it = history_->GetStates().rbegin(); it != history_->GetStates().rend(); ++it  )
        if( it->nStatus_ == status && it->startedTick_ > tickStarted )
            tickStarted = it->startedTick_;
    return ConvertTickToTimeString( tickStarted );
}
