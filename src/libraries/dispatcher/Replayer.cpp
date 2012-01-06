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
#include "Loader.h"
#include "Services.h"
#include "StaticModel.h"
#include "MemoryLogger_ABC.h"
#include "Shield.h"
#include "aar_plugin/AarPlugin.h"
#include "replay_plugin/ReplayPlugin.h"
#include "rights_plugin/RightsPlugin.h"
#include "score_plugin/ScorePlugin.h"
#include "messenger_plugin/MessengerPlugin.h"
#include <xeumeuleu/xml.hpp>

using namespace dispatcher;

namespace
{
    boost::shared_ptr< SimulationDispatcher > CreateSimulation( ClientPublisher_ABC& publisher, Model& model, CompositePlugin& handler )
    {
        boost::shared_ptr< SimulationDispatcher > result( new SimulationDispatcher( publisher, model ) );
        handler.AddHandler( result );
        return result;
    }
}

namespace
{
    class NullMemoryLogger : public MemoryLogger_ABC
    {
    public:
        NullMemoryLogger()
        {
            // NOTHING
        }
        virtual ~NullMemoryLogger()
        {
            // NOTHING
        }
        virtual void Update( const sword::ControlEndTick& /*message*/ )
        {
            // NOTHING
        }
        virtual void Update()
        {
            // NOTHING
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Replayer constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Replayer::Replayer( const Config& config )
    : services_        ( new Services() )
    , staticModel_     ( new StaticModel( config ) )
    , logger_          ( new ::NullMemoryLogger() )
    , model_           ( new Model( config, *staticModel_, *logger_ ) )
    , clientsNetworker_( new ClientsNetworker( config, handler_, *services_, *model_ ) )
    , simulation_      ( CreateSimulation( *clientsNetworker_, *model_, handler_ ) )
    , loader_          ( new Loader( *simulation_, handler_, config ) )
    , plugin_          ( new plugins::replay::ReplayPlugin( *model_, *clientsNetworker_, *clientsNetworker_, *loader_, *simulation_ ) )
    , shield_          ( new Shield( config, *model_, *clientsNetworker_, *clientsNetworker_ ) )
{
    handler_.AddHandler( model_ );
    handler_.AddHandler( clientsNetworker_ );
    // $$$$ AGE 2007-08-27: utiliser la PluginFactory => replay ESRI
    plugins::rights::RightsPlugin* rights = new plugins::rights::RightsPlugin( *model_, *clientsNetworker_, config, *clientsNetworker_, handler_, *clientsNetworker_, registrables_, 0 );
    handler_.Add( rights );
    handler_.Add( plugin_ );
    handler_.Add( new plugins::aar::AarPlugin( *clientsNetworker_, *rights, config ) );
    handler_.Add( new plugins::score::ScorePlugin( *clientsNetworker_, *clientsNetworker_, *clientsNetworker_, config, registrables_ ) );
    handler_.Add( new plugins::messenger::MessengerPlugin( *clientsNetworker_, *clientsNetworker_, *clientsNetworker_, config, registrables_ ) );
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
    shield_->Update( clientsNetworker_->IsAllowingConnections() );
}
