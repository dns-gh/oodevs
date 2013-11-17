// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LogisticPlugin.h"
#include "ConsignRecorder.h"
#include "FuneralResolver.h"
#include "MaintenanceResolver.h"
#include "MedicalResolver.h"
#include "SupplyResolver.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr_Gen.h"
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

enum E_ConsignEvent
{
    eConsignUnknown,
    eConsignCreation,
    eConsignUpdate,
    eConsignDestruction,
};

struct ConsignEvent
{
    ConsignEvent( LogisticPlugin::E_LogisticType t, E_ConsignEvent a, int i )
        : type( t ), action( a ), id( i )
    {
    }

    LogisticPlugin::E_LogisticType type;
    E_ConsignEvent action;
    int id;
};

template< typename T >
ConsignEvent MakeEvent( const T& msg, LogisticPlugin::E_LogisticType type, E_ConsignEvent action )
{
    return ConsignEvent( type, action,  msg.has_request() ? msg.request().id() : -1 );
}

#define MAKE_EVENT( M, T, E ) \
    if( msg.has_##M() ) return MakeEvent( msg.M(), LogisticPlugin::T, E )

ConsignEvent GetEventType( const sword::SimToClient& message )
{
    const auto& msg = message.message();
    MAKE_EVENT( log_funeral_handling_creation,    eLogisticType_Funeral, eConsignCreation );
    MAKE_EVENT( log_funeral_handling_update,      eLogisticType_Funeral, eConsignUpdate );
    MAKE_EVENT( log_funeral_handling_destruction, eLogisticType_Funeral, eConsignDestruction );

    MAKE_EVENT( log_maintenance_handling_creation,    eLogisticType_Maintenance, eConsignCreation );
    MAKE_EVENT( log_maintenance_handling_update,      eLogisticType_Maintenance, eConsignUpdate );
    MAKE_EVENT( log_maintenance_handling_destruction, eLogisticType_Maintenance, eConsignDestruction );

    MAKE_EVENT( log_medical_handling_creation,    eLogisticType_Medical, eConsignCreation );
    MAKE_EVENT( log_medical_handling_update,      eLogisticType_Medical, eConsignUpdate );
    MAKE_EVENT( log_medical_handling_destruction, eLogisticType_Medical, eConsignDestruction );

    MAKE_EVENT( log_supply_handling_creation,    eLogisticType_Supply, eConsignCreation );
    MAKE_EVENT( log_supply_handling_update,      eLogisticType_Supply, eConsignUpdate );
    MAKE_EVENT( log_supply_handling_destruction, eLogisticType_Supply, eConsignDestruction );

    return ConsignEvent( LogisticPlugin::eNbrLogisticType, eConsignUnknown, -1 );
}
#undef MAKE_EVENT

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
    : recorder_( new ConsignRecorder( archiveFile, 20*1024*1024, 100000 ) )
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
        if( tick >= 0 )
            currentTick_ = tick;
        const std::string time = message.message().control_begin_tick().date_time().data();        
        if( !time.empty() )
            simTime_ = time;
        return;
    }

    const auto ev = GetEventType( message );
    if( !recorder_->HasLogger( ev.type ) || ev.id <= 0 )
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
    recorder_->WriteEntry( ev.id, ev.action == eConsignDestruction,
            it->second->GetHistoryEntry() );
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

namespace
{

sword::EnumLogisticType GetLogisticType( LogisticPlugin::E_LogisticType type )
{
    switch( type )
    {
    case LogisticPlugin::eLogisticType_Funeral: return sword::log_funeral;
    case LogisticPlugin::eLogisticType_Maintenance: return sword::log_maintenance;
    case LogisticPlugin::eLogisticType_Medical: return sword::log_medical;
    case LogisticPlugin::eLogisticType_Supply: return sword::log_supply;
    };
    return sword::log_unknown;
}

}  // namespace

bool LogisticPlugin::HandleClientToSim( const sword::ClientToSim& msg,
        dispatcher::RewritingPublisher_ABC& unicaster, dispatcher::ClientPublisher_ABC& )
{
    if( !msg.message().has_logistic_history_request() )
        return false;
    const auto& rq = msg.message().logistic_history_request();
    sword::SimToClient reply;
    auto ack = reply.mutable_message()->mutable_logistic_history_ack();
    try
    {
        std::unordered_set< uint32_t > seen;
        for( int i = 0; i != rq.requests().size(); ++i )
        {
            const uint32_t requestId = rq.requests( i ).id();
            if( !seen.insert( requestId ).second )
                continue;
            auto it = consigns_.find( requestId );
            if( it == consigns_.end() )
                continue;
            const auto& history = it->second->GetHistory();
            for( auto ih = history.cbegin(); ih != history.cend(); ++ih )
            {
                if( ih->startTick_ < 0 || ih->status_ < 0 )
                    continue;
                auto st = ack->add_states();
                st->mutable_request()->set_id( requestId );
                st->mutable_id()->set_id( ih->id_ );
                st->set_type( GetLogisticType( it->second->GetType() ));
                st->set_start_tick( ih->startTick_ );
                if( ih->endTick_ >= 0 )
                    st->set_end_tick( ih->endTick_ );
                if( ih->handlerId_ > 0 )
                    st->mutable_handler()->set_id( ih->handlerId_ );
                st->set_status( ih->status_ );
            }
        }
    }
    catch( const std::exception& e )
    {
        reply.set_error_msg( tools::GetExceptionMsg( e ));
    }
    unicaster.Send( reply );
    return true;
}

LogisticPlugin* CreateLogisticPlugin(
    const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
    const tools::SessionConfig& config, xml::xistream& xis )
{
    return new LogisticPlugin(
        nameResolver,
        config.BuildSessionChildFile( "LogisticArchive" ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "maintenancefile", "LogMaintenance" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "supplyfile", "LogSupply" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "funeralfile", "LogFuneral" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "medicalfile", "LogMedical" ) ) );
}

}  // namespace logistic
}  // namespace plugins
