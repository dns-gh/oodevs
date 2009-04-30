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
#include "ProfileManager.h"
#include "Loader.h"
#include "ReplayPlugin.h"
#include "RightsPlugin.h"
#include "DispatcherPlugin.h"
#include "NoopPublisher.h"
#include "AarPlugin.h"
#include "Services.h"
#include "score_plugin/ScorePlugin.h"
#include <xeumeuleu/xml.h>

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
Replayer::Replayer( const Config& config )
    : registrables_    ()
    , services_        ( new Services() )
    , model_           ( CreateModel( handler_, config ) )
    , clientsNetworker_( new ClientsNetworker( config, handler_, *services_ ) )
    , simulation_      ( CreateSimulation( *clientsNetworker_, *model_, handler_ ) )
    , loader_          ( new Loader( *simulation_, handler_, config ) )
    , plugin_          ( new ReplayPlugin( *model_, *clientsNetworker_, *clientsNetworker_, *loader_, *simulation_ ) )
{
    handler_.AddHandler( clientsNetworker_ );

    // $$$$ AGE 2007-08-27: utiliser la PluginFactory => replay ESRI
    RightsPlugin* rights = new RightsPlugin( *model_, *clientsNetworker_, config, *clientsNetworker_, handler_, *clientsNetworker_, registrables_ );
    handler_.Add( rights  );
    handler_.Add( plugin_ );
    handler_.Add( new AarPlugin( *clientsNetworker_, *rights, config ) );
    handler_.Add( new plugins::score::ScorePlugin( *clientsNetworker_, *clientsNetworker_, *clientsNetworker_, config ) );
    handler_.Register( *services_ );
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
    handler_.Update();
}
