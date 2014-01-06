// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LogisticPlugin.h"
#include "ConsignCsvLogger.h"
#include "ConsignIndex.h"
#include "ConsignRecorder.h"
#include "protocol/Protocol.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/SessionConfig.h"
#include <tools/Exception.h>
#include <tools/Path.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <unordered_set>

namespace bpt = boost::posix_time;
namespace bg = boost::gregorian;

namespace plugins
{
namespace logistic
{

namespace
{

const size_t maxReturnedEntries = 500;
const size_t maxConsigns = 100000;
const size_t averageHistoryPerConsign = 15;

}  // namespace

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const boost::shared_ptr< ConsignCsvLogger >& logger,
        const tools::Path& archiveFile, bool load )
    // QA brigade benchmark reported around 17000 log lines, for all logistic
    // chains, over 55h of simulated time. This more than an order of magnitude
    // larger, being the number of requests instead of updates.
    : index_( load ? nullptr : new ConsignIndex() )
    , recorder_( load
        ? new ConsignRecorder( archiveFile, maxConsigns,
               averageHistoryPerConsign*maxConsigns )
        : new ConsignRecorder( archiveFile, 20*1024*1024, maxConsigns,
               averageHistoryPerConsign*maxConsigns ) )
    , logger_( logger )
    , currentTick_( 0 )
    , readOnly_( load )
{
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::~LogisticPlugin()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message )
{
    Receive( message, bpt::second_clock::local_time().date() );
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::Receive
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message, const bg::date& today )
{
    if( readOnly_ )
        return;

    if( message.message().has_control_begin_tick() )
    {
        recorder_->Flush();
        if( logger_ )
            logger_->Flush();

        const int tick = message.message().control_begin_tick().current_tick();
        if( tick >= 0 && tick > currentTick_ )
            currentTick_ = tick;
        const std::string time = message.message().control_begin_tick().date_time().data();
        if( !time.empty() )
            simTime_ = time;
        return;
    }

    const auto ev = index_->Update( message, currentTick_ );
    if( ev.id <= 0 )
        return;
    std::vector< uint32_t > entities;
    AppendConsignEntities( *ev.entry, entities );
    recorder_->WriteEntry( ev.id, ev.action == eConsignDestruction, *ev.entry, entities );
    if( logger_ )
        logger_->Log( ev, message, today, currentTick_, simTime_ );
}

template< typename M, typename R >
bool LogisticPlugin::HandleClientToSomething( const M& msg,
        dispatcher::RewritingPublisher_ABC& unicaster,
        dispatcher::ClientPublisher_ABC& )
{
    R reply;
    try
    {
        if( msg.message().has_logistic_history_request() )
        {
            HandleLogisticHistoryRequest( msg.message().logistic_history_request(),
                *reply.mutable_message()->mutable_logistic_history_ack() );
        }
        else if( msg.message().has_list_logistic_requests() )
        {
            HandleListLogisticRequests( msg.message().list_logistic_requests(),
                *reply.mutable_message()->mutable_list_logistic_requests_ack() );
        }
        else
            return false;
    }
    catch( const std::exception& e )
    {
        reply.set_error_msg( tools::GetExceptionMsg( e ));
    }
    unicaster.Send( reply );
    return true;
}

bool LogisticPlugin::HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& broadcaster )
{
    return HandleClientToSomething< sword::ClientToSim, sword::SimToClient >(
            msg, unicaster, broadcaster );
}

bool LogisticPlugin::HandleClientToReplay( const sword::ClientToReplay& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& broadcaster )
{
    return HandleClientToSomething< sword::ClientToReplay, sword::ReplayToClient >(
            msg, unicaster, broadcaster );
}

void LogisticPlugin::HandleLogisticHistoryRequest( const sword::LogisticHistoryRequest& rq,
        sword::LogisticHistoryAck& ack ) const
{
    boost::ptr_vector< sword::LogHistoryEntry > entries;
    std::unordered_set< uint32_t > seen;
    for( int i = 0; i != rq.requests().size(); ++i )
    {
        const uint32_t requestId = rq.requests( i ).id();
        if( !seen.insert( requestId ).second )
            continue;
        recorder_->GetHistory( requestId, entries );
        for( auto ie = entries.cbegin(); ie != entries.cend(); ++ie )
            *ack.add_entries() = *ie;
    }
}

void LogisticPlugin::HandleListLogisticRequests( const sword::ListLogisticRequests& rq,
        sword::ListLogisticRequestsAck& ack ) const
{
    size_t maxCount = maxReturnedEntries;
    if( rq.has_max_count() )
        maxCount = rq.max_count();
    int32_t currentTick = -1;
    if( rq.has_current_tick() )
        currentTick = rq.current_tick();
    std::set< uint32_t > entities;
    for( int i = 0; i != rq.entities().size(); ++i )
        entities.insert( rq.entities( i ).id() );

    boost::ptr_vector< sword::LogHistoryEntry > entries;
    GetRequestsFromEntities( *recorder_, entities, currentTick, maxCount, entries );
    for( auto ie = entries.begin(); ie != entries.end(); ie++ )
        *ack.add_entries() = *ie;
}

boost::shared_ptr< LogisticPlugin > CreateLogisticPlugin(
    const dispatcher::Model_ABC& model,
    const kernel::StaticModel& staticModel,
    const tools::SessionConfig& config )
{
    auto logger = CreateCsvLogger( model, staticModel, config );
    return boost::make_shared< LogisticPlugin >( logger,
        config.BuildSessionChildFile( "LogisticArchive" ), false );
}

boost::shared_ptr< LogisticPlugin > ReloadLogisticPlugin(
    const tools::SessionConfig& config )
{
    return boost::make_shared< LogisticPlugin >(
        boost::shared_ptr< ConsignCsvLogger >(),
        config.BuildSessionChildFile( "LogisticArchive" ), true );

}

}  // namespace logistic
}  // namespace plugins
