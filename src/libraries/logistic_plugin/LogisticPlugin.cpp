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
#include "dispatcher/ClientsNetworker.h"
#include "ENT/ENT_Tr_Gen.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/SessionConfig.h"
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#include "QtCore/qTranslator.h"
#include "QtCore/qLocale.h"
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

    typedef std::vector< ConsignResolver_ABC* >::iterator IT_ConsignResolvers;

    QString ReadLang()
    {
        QSettings settings( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }

    void InitTranslator( const QString& name, const QString& lang )
    {
        QTranslator* translator = new QTranslator( qApp );
        QString filename( name );
        filename += "_";
        filename += lang;
        if( translator->load( filename, "." ) || translator->load( filename, "resources/locales" ) )
            qApp->installTranslator( translator ); 
        else
            delete translator;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const std::string& maintenanceFile,
                                const std::string& supplyFile, const std::string& funeralFile, const std::string& medicalFile,
                                int maxHistoricFiles, int maxFileLines, dispatcher::ClientsNetworker& clients )
    : nameResolver_( nameResolver )
    , clients_( clients )
{
    QString lang = ReadLang();
    InitTranslator( "ENT", lang );
    InitTranslator( "logistic_plugin", lang );
    ENT_Tr::InitTranslations();

    resolvers_.resize( eNbrLogisticType );
    resolvers_[ eLogisticType_Maintenance ] = new MaintenanceResolver( maintenanceFile, *nameResolver, maxHistoricFiles, maxFileLines );
    resolvers_[ eLogisticType_Supply ]      = new SupplyResolver( supplyFile, *nameResolver, maxHistoricFiles, maxFileLines );
    resolvers_[ eLogisticType_Funeral ]     = new FuneralResolver( funeralFile, *nameResolver, maxHistoricFiles, maxFileLines );
    resolvers_[ eLogisticType_Medical ]     = new MedicalResolver( medicalFile, *nameResolver, maxHistoricFiles, maxFileLines );
    for( auto r = resolvers_.begin(); r != resolvers_.end(); ++r )
        (*r)->InitHeader();
    
    clients.RegisterMessage( *this, &LogisticPlugin::OnReceive );
    clients.RegisterMessage( *this, &LogisticPlugin::OnReceiveClientToMessenger );
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
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_begin_tick() )
    {
        int currentTick = message.message().control_begin_tick().current_tick();
        std::string simTime = message.message().control_begin_tick().date_time().data();        
        for( auto r = resolvers_.begin(); r != resolvers_.end(); ++r )
            (*r)->SetTime( currentTick, simTime );
    }
    else
    {
        for( auto r = resolvers_.begin(); r != resolvers_.end(); ++r )
            if( (*r)->Receive( message ) )
                break;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::OnReceive
// Created: LDC 2012-12-14
// -----------------------------------------------------------------------------
void LogisticPlugin::OnReceive( const sword::ClientToSim& message )
{
    if( message.message().has_log_history_request_for_play() )
    {
        sword::LogHistoryRequestForPlay request = message.message().log_history_request_for_play();
        for( auto r = resolvers_.begin(); r != resolvers_.end(); ++r )
            (*r)->ForceNewFile();
        sword::MessengerToClient messageToLauncher;
        sword::LogHistoryRequestForPlay* answer = messageToLauncher.mutable_message()->mutable_log_history_request_for_play();
        answer->mutable_date_time()->set_data( request.date_time().data() );
        answer->set_profile( request.profile() );
        answer->set_exercise( request.exercise() );
        answer->set_session( request.session() );
        clients_.Send( messageToLauncher );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::OnReceiveClientToMessenger
// Created: LDC 2012-12-17
// -----------------------------------------------------------------------------
void LogisticPlugin::OnReceiveClientToMessenger( const sword::ClientToMessenger& message )
{
    if( message.message().has_log_history_request_for_play_ack() )
    {
        auto request = message.message().log_history_request_for_play_ack();
        sword::SimToClient answer;
        auto redirectedMessage = answer.mutable_message()->mutable_log_history_request_for_play_ack();
        redirectedMessage->set_exercise( request.exercise() );
        redirectedMessage->set_session( request.session() );
        redirectedMessage->set_profile( request.profile() );
        redirectedMessage->mutable_date_time()->set_data( request.date_time().data() );
        clients_.Send( answer );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::GetConsignCount
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
int LogisticPlugin::GetConsignCount( E_LogisticType eLogisticType ) const
{
    size_t resolverIndex = static_cast< size_t >( eLogisticType );
    if( resolverIndex < resolvers_.size() )
    {
        const ConsignResolver_ABC* pResolver = resolvers_[ resolverIndex ];
        if( pResolver )
            return pResolver->GetConsignCount();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::SetMaxLinesInFile
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
void LogisticPlugin::SetMaxLinesInFile( int maxLines )
{
    for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
        (*r)->SetMaxLinesInFile( maxLines );
}


LogisticPlugin* CreateLogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const tools::SessionConfig& config, int maxHistoricFiles, int maxFileLines, dispatcher::ClientsNetworker& clients )
{
    return new LogisticPlugin(
        nameResolver,
        config.BuildSessionChildFile( "LogMaintenance" ),
        config.BuildSessionChildFile( "LogRavitaillement" ),
        config.BuildSessionChildFile( "LogFuneraire" ),
        config.BuildSessionChildFile( "LogMedical" ),
        maxHistoricFiles, maxFileLines, clients ); 
}

}  // namespace logistic
}  // namespace plugins