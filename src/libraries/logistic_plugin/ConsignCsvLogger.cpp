// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ConsignCsvLogger.h"
#include "ConsignResolver_ABC.h"
#include "FuneralResolver.h"
#include "MaintenanceResolver.h"
#include "MedicalResolver.h"
#include "NameResolver.h"
#include "SupplyResolver.h"
#include "clients_kernel/Tools.h"
#include "tools/Language.h"
#include "tools/SessionConfig.h"
#include <tools/Exception.h>
#include <tools/Path.h>
#pragma warning( push, 0 )
#include "QtCore/qTranslator.h"
#include <QtGui/qapplication.h>
#pragma warning( pop )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>

namespace bg = boost::gregorian;
using namespace plugins::logistic;

namespace
{

int localAppliArgc( 1 );
char* localAppliArgv[] = { " " };

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

ConsignCsvLogger::ConsignCsvLogger(
        const boost::shared_ptr< const NameResolver_ABC>& resolver,
        const tools::Path& maintenanceFile, const tools::Path& supplyFile,
        const tools::Path& funeralFile, const tools::Path& medicalFile )
    : nameResolver_( resolver )
    , localAppli_ ( !qApp ? new QApplication( localAppliArgc, localAppliArgv ) : 0 )
{
    if( qApp )
    {
        tools::AddTranslator( *qApp, tools::Language::Current(), "ENT" );
        tools::AddTranslator( *qApp, tools::Language::Current(), "logistic_plugin" );
    }
    ENT_Tr::InitTranslations();

    loggers_.insert( LogisticPlugin::eLogisticType_Maintenance,
        std::auto_ptr< ConsignResolver_ABC >(
            new ConsignResolver_ABC( maintenanceFile, GetMaintenanceHeader() )));
    loggers_.insert( LogisticPlugin::eLogisticType_Supply,
        std::auto_ptr< ConsignResolver_ABC >(
            new ConsignResolver_ABC( supplyFile, GetSupplyHeader() )));
    loggers_.insert( LogisticPlugin::eLogisticType_Funeral,
        std::auto_ptr< ConsignResolver_ABC >(
            new ConsignResolver_ABC( funeralFile, GetFuneralHeader() )));
    loggers_.insert( LogisticPlugin::eLogisticType_Medical,
        std::auto_ptr< ConsignResolver_ABC >(
            new ConsignResolver_ABC( medicalFile, GetMedicalHeader() )));
}

ConsignCsvLogger::~ConsignCsvLogger()
{
}

void ConsignCsvLogger::Log( const ConsignEvent& ev, const sword::SimToClient& message,
       const bg::date& today, int32_t tick, const std::string& simTime  )
{
    auto logger = loggers_.find( ev.type );
    if( logger == loggers_.end() )
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
    if( it->second->UpdateConsign( message, *nameResolver_, tick, simTime ) )
        logger->second->Write( it->second->ToString(), today );
    if( ev.action == eConsignDestruction )
        consigns_.erase( it );
}

void ConsignCsvLogger::Flush()
{
    for( auto it = loggers_.begin(); it != loggers_.end(); ++it )
        it->second->Flush();
}

void ConsignCsvLogger::SetMaxLinesInFile( int maxLines )
{
    for( auto it = loggers_.begin(); it != loggers_.end(); ++it )
        it->second->SetMaxLinesInFile( maxLines );
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::DebugGetConsignCount
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
int ConsignCsvLogger::DebugGetConsignCount(
        LogisticPlugin::E_LogisticType eLogisticType ) const
{
    int count = 0;
    for( auto it = consigns_.cbegin(); it != consigns_.cend(); ++it )
        if( it->second->GetType() == eLogisticType )
            ++count;
    return count;
}

boost::shared_ptr< ConsignCsvLogger > plugins::logistic::CreateCsvLogger(
    const dispatcher::Model_ABC& model,
    const kernel::StaticModel& staticModel,
    const tools::SessionConfig& config )
{
    auto names = boost::make_shared< NameResolver >( model, staticModel );
    return boost::make_shared< ConsignCsvLogger >(
        names,
        config.BuildSessionChildFile( "LogMaintenance" ),
        config.BuildSessionChildFile( "LogSupply" ),
        config.BuildSessionChildFile( "LogFuneral" ),
        config.BuildSessionChildFile( "LogMedical" ) );
}
