// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Dispatcher.h"
#include "Model.h"
#include "SimulationNetworker.h"
#include "ClientsNetworker.h"
#include "Config.h"
#include "Logger.h"
#include "PluginFactory.h"
#include "Services.h"
#include <google/protobuf/Message.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const Config& config )
    : config_( config )
    , model_( new Model( config_ ) )
    , registrables_( new CompositeRegistrable() )
    , handler_( new CompositePlugin( ) )
    , services_( new Services() )
    , clientsNetworker_( new ClientsNetworker( config_, *handler_, *services_ ) )
    , simulationNetworker_( new SimulationNetworker( *model_, *clientsNetworker_, *handler_, config_ ) )
    , factory_( new PluginFactory( config_, *model_, *simulationNetworker_, *clientsNetworker_, *handler_, *registrables_, *services_ ) )
    , logger_( new Logger() )
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    handler_->AddHandler( clientsNetworker_ );
    handler_->AddHandler( model_ );
}

// $$$$ AGE 2008-07-16: Les plugins / MessageHandlers doivent �tre enregistr�s dans un certain ordre
// $$$$ AGE 2008-07-16: Typiquement : 
// $$$$ AGE 2008-07-16:  * DispatcherPlugin pour forwarder aux clients
// $$$$ AGE 2008-07-16:  * Model pour les plugins l'utilisant, mais peut d�clencher des �v�nements sur Entity_ABC::Update
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
    clientsNetworker_   ->Update();
    simulationNetworker_->Update();
    handler_->Update();
    logger_->Update();
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
