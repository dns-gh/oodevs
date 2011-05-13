// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "CrossbowPlugin.h"
#include "CrossbowPublisher.h"
#include "dispatcher/DefaultProfile.h"
#include "tools/MessageDispatcher_ABC.h"
#include "tools/ClientNetworker.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/AuthenticationSenders.h"
#include <boost/thread.hpp>

using namespace plugins;
using namespace plugins::crossbow;

namespace
{
    class DummyClientNetworker : public tools::ClientNetworker
                               , public Publisher_ABC
    {
    public:
        DummyClientNetworker()
            : defaultProfile_( new dispatcher::DefaultProfile() )
        {}
        virtual ~DummyClientNetworker() {}

        virtual void Send( const sword::ClientToSim& /*message*/ ) {}
        virtual void Send( const sword::ClientToAuthentication& message )
        {
            MessageSender_ABC::Send( endpoint_, message );
        }
        virtual void Send( const sword::ClientToReplay& /*message*/ ) {}
        virtual void Send( const sword::ClientToAar& /*message*/ ) {}
        virtual void Send( const sword::ClientToMessenger& /*message*/ ) {}


    protected:
        virtual void ConnectionSucceeded( const std::string& endpoint )
        {
            ClientNetworker::ConnectionSucceeded( endpoint );
            endpoint_ = endpoint;
            authentication::AuthenticationRequest message;
            message().set_login( defaultProfile_->GetName().c_str() );
            message().set_password( "" );
            message.Send( *this );
            std::cout << "Dummy::ConnectionSucceeded " << endpoint << std::endl;
        }

    private:
        std::string endpoint_;
        std::auto_ptr< dispatcher::Profile_ABC > defaultProfile_;
    };
}

class CrossbowPlugin::ListenerThread : public boost::thread
{
public:
    ListenerThread( CrossbowPublisher& crossbow, int frequency )
        : boost::thread( boost::bind( &CrossbowPlugin::ListenerThread::Run, this ) )
        , crossbow_ ( crossbow ), frequency_ ( frequency ) {}

    void Run()
    {
        while ( true )
        {
            crossbow_.UpdateListeners();
            boost::this_thread::sleep( boost::posix_time::milliseconds( frequency_ ) );
        }
    }
private:
    CrossbowPublisher& crossbow_;
    const int frequency_;
};

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::CrossbowPlugin( const dispatcher::Config& config, xml::xistream& xis,
                                dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel,
                                dispatcher::SimulationPublisher_ABC& publisher,
                                dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& dispatcher,
                                dispatcher::LinkResolver_ABC& /*links*/, dispatcher::CompositeRegistrable& /*registrables*/ )
    : clientNetworker_  ( new DummyClientNetworker() )
{
    try
    {
        crossbowPublisher_.reset( new CrossbowPublisher( config, model, staticModel, publisher, xis ) );
        listeners_.reset( new ListenerThread( *crossbowPublisher_, 500 ) );
        MT_LOG_INFO_MSG( "CrossbowPlugin : registered." )
    }
    catch ( std::exception& ex )
    {
        MT_LOG_ERROR_MSG( "CrossbowPlugin : load failed - " + std::string( ex.what() ) )
    }
    dispatcher.RegisterMessage( *this, &CrossbowPlugin::OnReceiveMessengerToClient );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::~CrossbowPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::Update
// Created: JCR 2009-03-28
// -----------------------------------------------------------------------------
void CrossbowPlugin::Update()
{
    static bool connect = false;
    if( !connect )
    {
        // clientNetworker_->Connect( "localhost:10001", false );
        connect = true;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::Receive
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::Receive( const sword::SimToClient& asnMsg )
{
    if( crossbowPublisher_.get() )
        crossbowPublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::Send
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPlugin::Send( const sword::MessengerToClient& asnMsg )
{
    if( crossbowPublisher_.get() )
        crossbowPublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::OnReceiveMessengerToClient
// Created: JCR 2009-06-27
// -----------------------------------------------------------------------------
void CrossbowPlugin::OnReceiveMessengerToClient( const std::string& /*link*/, const sword::MessengerToClient& message )
{
    if( crossbowPublisher_.get() )
        crossbowPublisher_->Receive( message );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::NotifyClientAuthenticated
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::NotifyClientLeft
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}