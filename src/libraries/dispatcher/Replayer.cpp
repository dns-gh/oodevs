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
#include "ReplaySynchronizer.h"
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
#include "vision_plugin/VisionPlugin.h"
#include "tools/FileWrapper.h"
#include "protocol/Replay.h"
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

using namespace dispatcher;

namespace
{
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

    // Forwards all SimToClient messages it receives to ClientPublisher
    struct ReceiverToSender : public PluginContainer
    {
        explicit ReceiverToSender( ClientPublisher_ABC& publisher )
            : publisher_( publisher )
        {}
        virtual void Receive( const sword::SimToClient& message )
        {
            publisher_.Send( message );
        }
    private:
        ClientPublisher_ABC& publisher_;
    };
}

// Forwards all messages to ClientPublisher, excepting SimToClient messages
// forwarded to its children, as a PluginContainer.
struct Replayer::SenderToReceiver : public ClientPublisher_ABC
                                  , public PluginContainer
{
    explicit SenderToReceiver( ClientPublisher_ABC& publisher )
        : publisher_( publisher )
    {}
    virtual void Send( const sword::SimToClient& message )
    {
        Receive( message );
    }
    virtual void Send( const sword::AuthenticationToClient& message ) { publisher_.Send( message ); }
    virtual void Send( const sword::ReplayToClient&         message ) { publisher_.Send( message ); }
    virtual void Send( const sword::AarToClient&            message ) { publisher_.Send( message ); }
    virtual void Send( const sword::MessengerToClient&      message ) { publisher_.Send( message ); }
    virtual void Send( const sword::DispatcherToClient&     message ) { publisher_.Send( message ); }
private:
    ClientPublisher_ABC& publisher_;
};

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
    , modelHandler_    ( new SenderToReceiver( *clientsNetworker_ ) )
    , synchronizer_    ( new ReplaySynchronizer( *modelHandler_, *model_ ) )
    , loader_          ( new Loader( *synchronizer_, handler_, config, *clientsNetworker_ ) )
    , replay_          ( new plugins::replay::ReplayPlugin( *model_, *clientsNetworker_, *clientsNetworker_, *loader_, *synchronizer_ ) )
    , publisher_       ( new NullPublisher() )
    , started_         ( false )
    , rights_          ( boost::make_shared< plugins::rights::RightsPlugin >(
                            *model_, *clientsNetworker_, config, *clientsNetworker_,
                            handler_, *clientsNetworker_, registrables_, 0, true ) )
    , stopped_         ( false )
    , log_             ( config.BuildSessionChildFile( "Protobuf_replay.log" ), config.GetDispatcherProtobufLogFiles(),
                         config.GetDispatcherProtobufLogSize(), true, config.IsDispatcherProtobufLogInBytes() )
{
    clientsNetworker_->RegisterMessage( MakeLogger( log_, "Dispatcher received: ", *this, &Replayer::ReceiveClientToReplay ) );
    clientsNetworker_->RegisterMessage( MakeLogger( log_, "Dispatcher received: ", *this, &Replayer::ReceiveClientToSim ) );

    // Model synchronization pipeline configuration, with a first vision plugin to filter vision cones messages
    auto vision = boost::make_shared< plugins::vision::VisionPlugin >( *model_, *clientsNetworker_, *publisher_, *rights_ );
    vision->AddHandler( boost::make_shared< ReceiverToSender >( *clientsNetworker_ ) );
    modelHandler_->Add( vision );

    // Message loader pipeline configuration, with a second vision plugin to also filter vision cones messages
    handler_.AddHandler( model_ );
    vision = boost::make_shared< plugins::vision::VisionPlugin >( *model_, *clientsNetworker_, *publisher_, *rights_ );
    vision->AddHandler( clientsNetworker_ );
    vision->AddHandler( boost::make_shared< SimulationDispatcher >( *clientsNetworker_, *synchronizer_ ) );
    handler_.Add( vision );
    handler_.Add( rights_ );
    handler_.Add( replay_ );
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
bool Replayer::Update()
{
    if( !started_ )
        started_ = loader_->Start();
    clientsNetworker_->Update();
    handler_.Update();
    return !stopped_;
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
        sword::ClientToReplay& msg )
{
    dispatcher::UnicastPublisher unicaster( rights_->GetAuthenticatedPublisher( link ),
            link, rights_->GetClientID( link ), msg.context() );
    if( msg.message().has_control_stop() )
    {
        stopped_ = true;
        sword::ReplayToClient ack;
        ack.mutable_message()->mutable_control_stop_ack()
            ->set_error_code( sword::ControlAck::no_error );
        unicaster.Send( ack );
    }
    handler_.HandleClientToReplay( msg, unicaster, *clientsNetworker_ );
}

// -----------------------------------------------------------------------------
// Name: Replayer::ReceiveClientToSim
// Created: SLI 2014-10-14
// -----------------------------------------------------------------------------
void Replayer::ReceiveClientToSim( const std::string&, sword::ClientToSim& )
{
    // NOTHING
}
