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
#include "PluginFactory.h"
#include "Config.h"
#include "Services.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dispatcher constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Dispatcher::Dispatcher( const Config& config )
    : config_( config )
    , handler_()
    , registrables_()
    , services_( new Services() )
    , model_( new Model( config_ ) )
    , clientsNetworker_( new ClientsNetworker( config_, handler_, *services_ ) )
    , simulationNetworker_( new SimulationNetworker( *model_, *clientsNetworker_, handler_, config_ ) )
    , factory_( new PluginFactory( config_, *model_, *simulationNetworker_, *clientsNetworker_, handler_, registrables_ ) )
{
    handler_.AddHandler( clientsNetworker_ );
    handler_.AddHandler( model_ );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Dispatcher::Update()
{
    clientsNetworker_   ->Update();
    simulationNetworker_->Update();
    handler_.Update();
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
    handler_.Register( *services_ );
}
