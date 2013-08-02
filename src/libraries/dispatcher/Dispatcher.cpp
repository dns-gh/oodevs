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
#include "MT_Tools/MT_Logger.h"
#include <google/protobuf/message.h>

#pragma warning( disable: 4355 )

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const Config& config, int maxConnections )
    : staticModel_        ( new StaticModel( config ) )
    , memoryLogger_       ( new MemoryLogger() )
    , model_              ( new Model( config, *staticModel_, *memoryLogger_ ) )
    , handler_            ( new CompositePlugin() )
    , registrables_       ( new CompositeRegistrable() )
    , services_           ( new Services() )
    , logFactory_         ( config.IsDispatcherProtobufLogInBytes() )
    , log_                ( logFactory_, config.BuildSessionChildFile( "Protobuf.log" ), config.GetDispatcherProtobufLogFiles(), config.GetDispatcherProtobufLogSize() )
    , clientsNetworker_   ( new ClientsNetworker( config, *handler_, *services_, *model_ ) )
    , simulationNetworker_( new SimulationNetworker( *model_, *clientsNetworker_, *handler_, config, log_ ) )
    , factory_            ( new PluginFactory( config, *model_, *staticModel_, *simulationNetworker_, *clientsNetworker_, *handler_, *registrables_, *services_, log_, maxConnections ) )
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    handler_->Add( clientsNetworker_ );
    handler_->AddHandler( model_ );
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
    factory_->Close();
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
