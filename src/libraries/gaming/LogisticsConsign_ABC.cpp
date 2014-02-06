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
#include "protocol/MessageParameters.h"

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
// Name: LogisticsConsign_ABC NeedResolution
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
bool LogisticsConsign_ABC::NeedResolution() const
{
    return false;
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
        const google::protobuf::RepeatedPtrField< sword::LogHistoryEntry >& history,
        unsigned int currentTick )
{
    history_->Clear();
    for( int i = start; i != end; ++i )
    {
        const auto& msg = history.Get( i );
        if( static_cast< unsigned int >( msg.tick() ) > currentTick )
            continue;

        HistoryState state;
        if( !UpdateHistoryState( msg, state ) )
        {
            if( msg.has_funeral() )
            {
                const auto& sub = msg.funeral();
                if( sub.has_destruction() || !sub.has_update() )
                    // Creation messages are not really interesting
                    continue;
                if( sub.update().has_state() )
                    state.nStatus_ = sub.update().state();
                state.handler_ = GetRequestHandler(
                    protocol::GetParentEntityId( sub.update().handling_unit() ));
            }
            else if( msg.has_maintenance() )
            {
                const auto& sub = msg.maintenance();
                if( sub.has_destruction() || !sub.has_update() )
                    continue;
                if( sub.update().has_state() )
                    state.nStatus_ = sub.update().state();
                state.handler_ = GetRequestHandler( sub.update().provider().id() );
            }
            else if( msg.has_medical() )
            {
                const auto& sub = msg.medical();
                if( sub.has_destruction() || !sub.has_update() )
                    continue;
                if( sub.update().has_state() )
                    state.nStatus_ = sub.update().state();
                state.handler_ = GetRequestHandler( sub.update().provider().id() );
            }
            else
                continue;
        }
        state.startedTick_ = msg.tick();
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

// -----------------------------------------------------------------------------
// Name: LogisticsConsign_ABC::UpdateHistoryState
// Created: LGY 2014-01-30
// -----------------------------------------------------------------------------
bool LogisticsConsign_ABC::UpdateHistoryState( const sword::LogHistoryEntry& /*entry*/, HistoryState& /*state*/ )
{
    return false;
}
