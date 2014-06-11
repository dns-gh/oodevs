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
#include "SimulationPublisher_ABC.h"
#include "Loader.h"
#include "Services.h"
#include "StaticModel.h"
#include "MemoryLogger_ABC.h"
#include "aar_plugin/AarPlugin.h"
#include "replay_plugin/ReplayPlugin.h"
#include "rights_plugin/RightsPlugin.h"
#include "score_plugin/ScorePlugin.h"
#include "logistic_plugin/LogisticPlugin.h"
#include "messenger_plugin/MessengerPlugin.h"
#include "web_control_plugin/WebPlugin.h"
#include "tools/FileWrapper.h"
#include "protocol/Replay.h"
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

using namespace dispatcher;

namespace
{
    boost::shared_ptr< SimulationDispatcher > CreateSimulation( ClientPublisher_ABC& publisher, Model& model, PluginContainer& handler )
    {
        boost::shared_ptr< SimulationDispatcher > result( new SimulationDispatcher( publisher, model ) );
        handler.AddHandler( result );
        return result;
    }

    struct NullMemoryLogger : MemoryLogger_ABC
    {
        virtual void Update( const sword::ControlEndTick& )
        {}
        virtual void Update()
        {}
    };

    struct NullPublisher : SimulationPublisher_ABC
    {
        virtual void Send( const sword::ClientToSim& )
        {}
        virtual void Send( const sword::DispatcherToSim& )
        {}
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
    , loader_          ( new Loader( *simulation_, handler_, config, *clientsNetworker_ ) )
    , plugin_          ( new plugins::replay::ReplayPlugin( *model_, *clientsNetworker_, *clientsNetworker_, *loader_, *simulation_ ) )
    , publisher_       ( new NullPublisher() )
    , started_         ( false )
    , rights_          ( boost::make_shared< plugins::rights::RightsPlugin >(
        *model_, *clientsNetworker_, config, *clientsNetworker_,
        handler_, *clientsNetworker_, registrables_, 0 ))
{
    clientsNetworker_->RegisterMessage( *this, &Replayer::ReceiveClientToReplay );

    handler_.AddHandler( model_ );
    handler_.AddHandler( clientsNetworker_ );
    handler_.Add( rights_ );
    handler_.Add( plugin_ );
    handler_.Add( boost::make_shared< plugins::aar::AarPlugin >( *clientsNetworker_, *rights_, config ) );
    handler_.Add( boost::make_shared< plugins::score::ScorePlugin >(
                *clientsNetworker_, *clientsNetworker_, *clientsNetworker_, config, registrables_ ) );
    handler_.Add( boost::make_shared< plugins::messenger::MessengerPlugin >(
                *clientsNetworker_, *clientsNetworker_, *clientsNetworker_, config, registrables_ ) );
    handler_.Add( plugins::logistic::ReloadLogisticPlugin( config ) );
    tools::Xifstream xis( config.GetSessionFile() );
    xis >> xml::start( "session" )
            >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "plugins" )
                        >> xml::list("web_control", *this, &Replayer::OnWebControl );
    handler_.Register( *services_ );
    started_ = loader_->Start();
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
    if( !started_ )
        started_ = loader_->Start();
    clientsNetworker_->Update();
    handler_.Update();
}

// -----------------------------------------------------------------------------
// Name: Replayer::OnWebControl
// Created: BAX 2012-08-20
// -----------------------------------------------------------------------------
void Replayer::OnWebControl( xml::xistream& xis )
{
    handler_.Add( boost::make_shared< plugins::web_control::WebPlugin >( *publisher_, xis ) );
}

void Replayer::ReceiveClientToReplay( const std::string& link,
        const sword::ClientToReplay& msg )
{
    dispatcher::UnicastPublisher unicaster( rights_->GetAuthenticatedPublisher( link ), link,
            rights_->GetClientID( link ), msg.context() );
    handler_.HandleClientToReplay( msg, unicaster, *clientsNetworker_ );
}
