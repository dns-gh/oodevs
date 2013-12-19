// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LogisticPlugin.h"
#include "ConsignIndex.h"
#include "ConsignRecorder.h"
#include "FuneralResolver.h"
#include "MaintenanceResolver.h"
#include "MedicalResolver.h"
#include "NameResolver.h"
#include "SupplyResolver.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/Language.h"
#include "tools/SessionConfig.h"
#include "tools/XmlStreamOperators.h"
#include <tools/Exception.h>
#include <tools/Path.h>
#pragma warning( push, 0 )
#include "QtCore/qTranslator.h"
#include <QtGui/qapplication.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
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

int localAppliArgc( 1 );
char* localAppliArgv[] = { " " };
const size_t maxReturnedEntries = 500;
const size_t maxConsigns = 100000;
const size_t averageHistoryPerConsign = 15;

std::auto_ptr< ConsignData_ABC > NewConsign( LogisticPlugin::E_LogisticType type, int id )
{
    const auto idString = boost::lexical_cast< std::string >( id );
    switch( type )
    {
        case LogisticPlugin::eLogisticType_Funeral:
            return std::auto_ptr< ConsignData_ABC >( new FuneralConsignData( idString ) );
        case LogisticPlugin::eLogisticType_Maintenance:
            return std::auto_ptr< ConsignData_ABC >( new MaintenanceConsignData( idString ) );
        case LogisticPlugin::eLogisticType_Medical:
            return std::auto_ptr< ConsignData_ABC >( new MedicalConsignData( idString ) );
        case LogisticPlugin::eLogisticType_Supply:
            return std::auto_ptr< ConsignData_ABC >( new SupplyConsignData( idString ) );
    };
    throw MASA_EXCEPTION( "unsupported logistic consign type" );
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const boost::shared_ptr< const NameResolver_ABC >& nameResolver,
        const tools::Path& archiveFile, const tools::Path& maintenanceFile,
        const tools::Path& supplyFile, const tools::Path& funeralFile,
        const tools::Path& medicalFile )
    // QA brigade benchmark reported around 17000 log lines, for all logistic
    // chains, over 55h of simulated time. This more than an order of magnitude
    // larger, being the number of requests instead of updates.
    : index_( new ConsignIndex() )
    , recorder_( new ConsignRecorder( archiveFile, 20*1024*1024, maxConsigns,
               averageHistoryPerConsign*maxConsigns ) )
    , nameResolver_( nameResolver )
    , localAppli_ ( !qApp ? new QApplication( localAppliArgc, localAppliArgv ) : 0 )
    , currentTick_( 0 )
{
    if( qApp )
    {
        tools::AddTranslator( *qApp, tools::Language::Current(), "ENT" );
        tools::AddTranslator( *qApp, tools::Language::Current(), "logistic_plugin" );
    }
    ENT_Tr::InitTranslations();

    recorder_->AddLogger( eLogisticType_Maintenance, maintenanceFile, GetMaintenanceHeader() );
    recorder_->AddLogger( eLogisticType_Supply, supplyFile, GetSupplyHeader() );
    recorder_->AddLogger( eLogisticType_Funeral, funeralFile, GetFuneralHeader() );
    recorder_->AddLogger( eLogisticType_Medical, medicalFile, GetMedicalHeader() );
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
    if( message.message().has_control_begin_tick() )
    {
        recorder_->Flush();

        const int tick = message.message().control_begin_tick().current_tick();
        if( tick >= 0 && tick > currentTick_ )
            currentTick_ = tick;
        const std::string time = message.message().control_begin_tick().date_time().data();
        if( !time.empty() )
            simTime_ = time;
        return;
    }

    std::vector< uint32_t > entities;
    const auto ev = index_->Update( message, entities, currentTick_ );
    if( ev.id <= 0 )
        return;
    recorder_->WriteEntry( ev.id, ev.action == eConsignDestruction, *ev.entry, entities );

    // Push to the legacy logger
    if( !recorder_->HasLogger( ev.type ) )
        return;
    auto it = consigns_.find( ev.id );
    if( ev.action == eConsignCreation || it == consigns_.end() )
    {
        auto consign = NewConsign( ev.type, ev.id );
        if( it == consigns_.end() )
            it = consigns_.insert( ev.id, consign ).first;
        else
            consigns_.replace( it, consign );
    }
    if( it->second->UpdateConsign( message, *nameResolver_, currentTick_, simTime_ ) )
        recorder_->Write( ev.type, it->second->ToString(), today );
    if( ev.action == eConsignDestruction )
        consigns_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::DebugGetConsignCount
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
int LogisticPlugin::DebugGetConsignCount( E_LogisticType eLogisticType ) const
{
    int count = 0;
    for( auto it = consigns_.cbegin(); it != consigns_.cend(); ++it )
        if( it->second->GetType() == eLogisticType )
            ++count;
    return count;
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::SetMaxLinesInFile
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
void LogisticPlugin::SetMaxLinesInFile( int maxLines )
{
    recorder_->SetMaxLinesInFile( maxLines );
}

bool LogisticPlugin::HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& )
{
    sword::SimToClient reply;
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
    int32_t startTick = -1;
    if( rq.has_start_tick() )
        startTick = rq.start_tick();
    std::set< uint32_t > entities;
    for( int i = 0; i != rq.entities().size(); ++i )
        entities.insert( rq.entities( i ).id() );

    boost::ptr_vector< sword::LogHistoryEntry > entries;
    GetRequestsFromEntities( *recorder_, entities, startTick, maxCount, entries );
    for( auto ie = entries.begin(); ie != entries.end(); ie++ )
        *ack.add_entries() = *ie;
}

boost::shared_ptr< LogisticPlugin > CreateLogisticPlugin(
    const dispatcher::Model_ABC& model,
    const kernel::StaticModel& staticModel,
    const tools::SessionConfig& config )
{
    auto names = boost::make_shared< NameResolver >( model, staticModel );
    return boost::make_shared< LogisticPlugin >(
        names,
        config.BuildSessionChildFile( "LogisticArchive" ),
        config.BuildSessionChildFile( "LogMaintenance" ),
        config.BuildSessionChildFile( "LogSupply" ),
        config.BuildSessionChildFile( "LogFuneral" ),
        config.BuildSessionChildFile( "LogMedical" ) );
}

}  // namespace logistic
}  // namespace plugins
