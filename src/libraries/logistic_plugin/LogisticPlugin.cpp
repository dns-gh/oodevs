// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LogisticPlugin.h"
#include "ConsignData_ABC.h"
#include "FuneralResolver.h"
#include "MaintenanceResolver.h"
#include "MedicalResolver.h"
#include "SupplyResolver.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr_Gen.h"
#include "protocol/Protocol.h"
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

ConsignEvent GetEventType( const sword::SimToClient& message )
{
    const auto& msg = message.message();
    if( msg.has_log_funeral_handling_creation() )
        return MakeEvent( msg.log_funeral_handling_creation(),
                LogisticPlugin::eLogisticType_Funeral, eConsignCreation );
    if( msg.has_log_funeral_handling_update() )
        return MakeEvent( msg.log_funeral_handling_update(),
            LogisticPlugin::eLogisticType_Funeral, eConsignUpdate );
    if( msg.has_log_funeral_handling_destruction() )
        return MakeEvent( msg.log_funeral_handling_destruction(),
            LogisticPlugin::eLogisticType_Funeral, eConsignDestruction );

    if( msg.has_log_maintenance_handling_creation() )
        return MakeEvent( msg.log_maintenance_handling_creation(),
            LogisticPlugin::eLogisticType_Maintenance, eConsignCreation );
    if( msg.has_log_maintenance_handling_update() )
        return MakeEvent( msg.log_maintenance_handling_update(),
            LogisticPlugin::eLogisticType_Maintenance, eConsignUpdate );
    if( msg.has_log_maintenance_handling_destruction() )
        return MakeEvent( msg.log_maintenance_handling_destruction(),
            LogisticPlugin::eLogisticType_Maintenance, eConsignDestruction );

    if( msg.has_log_medical_handling_creation() )
        return MakeEvent( msg.log_medical_handling_creation(),
            LogisticPlugin::eLogisticType_Medical, eConsignCreation );
    if( msg.has_log_medical_handling_update() )
        return MakeEvent( msg.log_medical_handling_update(),
            LogisticPlugin::eLogisticType_Medical, eConsignUpdate );
    if( msg.has_log_medical_handling_destruction() )
        return MakeEvent( msg.log_medical_handling_destruction(),
            LogisticPlugin::eLogisticType_Medical, eConsignDestruction );

    if( msg.has_log_supply_handling_creation() )
        return MakeEvent( msg.log_supply_handling_creation(),
            LogisticPlugin::eLogisticType_Supply, eConsignCreation );
    if( msg.has_log_supply_handling_update() )
        return MakeEvent( msg.log_supply_handling_update(),
            LogisticPlugin::eLogisticType_Supply, eConsignUpdate );
    if( msg.has_log_supply_handling_destruction() )
        return MakeEvent( msg.log_supply_handling_destruction(),
            LogisticPlugin::eLogisticType_Supply, eConsignDestruction );

    return ConsignEvent( LogisticPlugin::eNbrLogisticType, eConsignUnknown, -1 );
}

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
LogisticPlugin::LogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const tools::Path& maintenanceFile,
                                const tools::Path& supplyFile, const tools::Path& funeralFile, const tools::Path& medicalFile )
    : nameResolver_( nameResolver )
    , localAppli_ ( !qApp ? new QApplication( localAppliArgc, localAppliArgv ) : 0 )
    , currentTick_( 0 )
{
    if( qApp )
    {
        tools::AddTranslator( *qApp, tools::Language::Current(), "ENT" );
        tools::AddTranslator( *qApp, tools::Language::Current(), "logistic_plugin" );
    }
    ENT_Tr::InitTranslations();

    resolvers_.resize( eNbrLogisticType );
    resolvers_[ eLogisticType_Maintenance ] = new ConsignResolver_ABC( maintenanceFile, GetMaintenanceHeader() );
    resolvers_[ eLogisticType_Supply ]      = new ConsignResolver_ABC( supplyFile, GetSupplyHeader() );
    resolvers_[ eLogisticType_Funeral ]     = new ConsignResolver_ABC( funeralFile, GetFuneralHeader() );
    resolvers_[ eLogisticType_Medical ]     = new ConsignResolver_ABC( medicalFile, GetMedicalHeader() );
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::~LogisticPlugin()
{
    for( auto r = resolvers_.begin(); r != resolvers_.end(); ++r )
        delete *r;
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
        const int tick = message.message().control_begin_tick().current_tick();
        if( tick >= 0 )
            currentTick_ = tick;
        const std::string time = message.message().control_begin_tick().date_time().data();        
        if( !time.empty() )
            simTime_ = time;
        return;
    }

    const auto ev = GetEventType( message );
    if( static_cast< size_t >( ev.type ) >= resolvers_.size() || ev.id <= 0 )
        return;
    auto resolver = resolvers_[ static_cast< size_t >( ev.type ) ];

    auto it = consigns_.find( ev.id );
    if( ev.action == eConsignCreation || it == consigns_.end() )
    {
        auto consign = NewConsign( ev.type, ev.id );
        if( it == consigns_.end() )
            it = consigns_.insert( ev.id, consign ).first;
        else
            consigns_.replace( it, consign );
    } 
    it->second->SetTime( currentTick_, simTime_ );
    if( it->second->UpdateConsign( message, *nameResolver_ ) )
        resolver->Write( it->second->ToString(), today );
    if( ev.action == eConsignDestruction )
        consigns_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::GetConsignCount
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
    for( auto r = resolvers_.begin(); r != resolvers_.end(); ++r )
        (*r)->SetMaxLinesInFile( maxLines );
}


LogisticPlugin* CreateLogisticPlugin(
    const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
    const tools::SessionConfig& config, xml::xistream& xis )
{
    return new LogisticPlugin(
        nameResolver,
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "maintenancefile", "LogMaintenance" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "supplyfile", "LogSupply" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "funeralfile", "LogFuneral" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "medicalfile", "LogMedical" ) ) );
}

}  // namespace logistic
}  // namespace plugins
