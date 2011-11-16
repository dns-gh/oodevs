// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Dispatcher.h"
#include "Model.h"
#include "SimulationNetworker.h"
#include "ClientsNetworker.h"
#include "Config.h"
#include "MemoryLogger.h"
#include "PluginFactory.h"
#include "Services.h"
#include "StaticModel.h"
#include "Shield.h"
#include "LogFactory.h"
#include "clients_kernel/Tools.h"
#include "MT_Tools/MT_Logger.h"
#include <google/protobuf/message.h>
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#pragma warning( pop )

#pragma warning( disable: 4355 )

using namespace dispatcher;

namespace
{
    LogFactory factory;
}

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const Config& config, int maxConnections )
    : staticModel_        ( new StaticModel( config ) )
    , memoryLogger_       ( new MemoryLogger() )
    , model_              ( new Model( config, *staticModel_, *memoryLogger_ ) )
    , registrables_       ( new CompositeRegistrable() )
    , handler_            ( new CompositePlugin() )
    , services_           ( new Services() )
    , log_                ( factory, config.BuildSessionChildFile( "Protobuf.log" ), config.GetLogFiles(), config.GetLogSize() )
    , clientsNetworker_   ( new ClientsNetworker( config, *handler_, *services_ ) )
    , simulationNetworker_( new SimulationNetworker( *model_, *clientsNetworker_, *handler_, config, log_ ) )
    , shield_             ( new Shield( config, *clientsNetworker_, *clientsNetworker_ ) )
    , factory_            ( new PluginFactory( config, *model_, *staticModel_, *simulationNetworker_, *clientsNetworker_, *handler_, *registrables_, *services_, log_, maxConnections ) )
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    handler_->Add( clientsNetworker_ );
    handler_->AddHandler( model_ );
    int argc = 0;
    qapp_.reset( new Application_ABC( argc, 0 ) );
    AddTranslations();
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::AddTranslations
// Created: HBD 2010-06-28
// -----------------------------------------------------------------------------
void Dispatcher::AddTranslations()
{
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    QString locale = settings.readEntry( "/Common/Language", QTextCodec::locale() );
    qapp_->AddTranslator( locale, "messenger" );
}

// $$$$ AGE 2008-07-16: Les plugins / MessageHandlers doivent être enregistrés dans un certain ordre
// $$$$ AGE 2008-07-16: Typiquement :
// $$$$ AGE 2008-07-16:  * DispatcherPlugin pour forwarder aux clients
// $$$$ AGE 2008-07-16:  * Model pour les plugins l'utilisant, mais peut déclencher des évènements sur Entity_ABC::Update
// $$$$ AGE 2008-07-16:  * SaverPlugin (justement car utilise le model)

// -----------------------------------------------------------------------------
// Name: Dispatcher destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::~Dispatcher()
{
    google::protobuf::ShutdownProtobufLibrary();
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::Update()
{
    clientsNetworker_->Update();
    simulationNetworker_->Update();
    handler_->Update();
    shield_->Update();
    memoryLogger_->Update();
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::RegisterPluginFactory
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void Dispatcher::RegisterPluginFactory( PluginFactory_ABC& factory )
{
    factory_->Register( factory );
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::CreatePlugins
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void Dispatcher::CreatePlugins()
{
    factory_->Instanciate();
    handler_->Register( *services_ );
}
