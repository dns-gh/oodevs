// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Replayer.h"
#include "Config.h"
#include "Model.h"
#include "ClientsNetworker.h"
#include "SimulationDispatcher.h"
#include "LoaderFacade.h"
#include "ProfileManager.h"
#include "Loader.h"
#include "ReplayPlugin.h"
#include "RightsPlugin.h"
#include "DispatcherPlugin.h"
#include "NoopPublisher.h"

#include "xeumeuleu/xml.h"

using namespace dispatcher;

namespace 
{
    boost::shared_ptr< Model > CreateModel( CompositePlugin& handler, const Config& config )
    {
        boost::shared_ptr< Model > result( new Model( config ) );
        handler.AddHandler( result );
        return result;
    }
    boost::shared_ptr< SimulationDispatcher > CreateSimulation( ClientPublisher_ABC& publisher, Model& model, CompositePlugin& handler )
    {
        boost::shared_ptr< SimulationDispatcher > result( new SimulationDispatcher( publisher, model ) );
        handler.AddHandler( result );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Replayer constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Replayer::Replayer( const Config& config, const std::string& records )
    : model_           ( CreateModel( handler_, config ) )
    , clientsNetworker_( new ClientsNetworker( config, handler_ ) )
    , simulation_      ( CreateSimulation( *clientsNetworker_, *model_, handler_ ) )
    , loader_          ( new Loader( *simulation_, handler_, config, records ) )
    , facade_          ( new LoaderFacade( *clientsNetworker_, *loader_ ) )
    , plugin_          ( new ReplayPlugin( *facade_, *clientsNetworker_ ) )
{
    handler_.AddHandler( clientsNetworker_ );

    // $$$$ AGE 2007-08-27: utiliser la PluginFactory => replay ESRI
    RightsPlugin* rights = new RightsPlugin( *model_, *clientsNetworker_, config, *clientsNetworker_, handler_ );
    handler_.Add( rights  );
    static NoopSimulationPublisher simu;
    handler_.Add( new DispatcherPlugin( *model_, simu, *clientsNetworker_, *rights ) );
    handler_.Add( plugin_ );
    loader_->Start();
}
 
// -----------------------------------------------------------------------------
// Name: Replayer destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Replayer::~Replayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Replayer::Update
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Replayer::Update()
{
    clientsNetworker_->Update();
    facade_->Update();
}
