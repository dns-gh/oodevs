// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
#include "dispatcher/Logger_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )

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
        virtual void ConnectionError( const std::string& endpoint, const std::string& reason )
        {
            std::cerr << "Dummy::ConnectionError " << endpoint << " : " << reason << std::endl;
        }
        virtual void ConnectionWarning( const std::string& endpoint, const std::string& reason )
        {
            std::cerr << "Dummy::ConnectionWarning " << endpoint << " : " << reason << std::endl;
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
        for( ;; )
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
CrossbowPlugin::CrossbowPlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, 
                                dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Config& config, 
                                xml::xistream& xis, dispatcher::Logger_ABC& logger )
    : clientNetworker_  ( new DummyClientNetworker() )
{
    try
    {
        crossbowPublisher_.reset( new CrossbowPublisher( config, model, staticModel, publisher, xis, logger ) );
        listeners_.reset( new ListenerThread( *crossbowPublisher_, 500 ) );
        logger.LogInfo( "CrossbowPlugin : registered." );
    }
    catch ( std::exception& ex )
    {
        logger.LogError( "CrossbowPlugin : load failed - " + std::string( ex.what() ) );
    }
    // dispatcher.RegisterMessage( *this, &CrossbowPlugin::OnReceiveMessengerToClient );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::~CrossbowPlugin()
{
    listeners_->interrupt();
    listeners_->join();
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
