// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "client_proxy_test_pch.h"
#include "client_proxy/SwordConnectionHandler_ABC.h"
#include "client_proxy/SwordProxy.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/Aar.h"
#include "protocol/Replay.h"
#include "protocol/Messenger.h"
#include "protocol/Simulation.h"
#include "tools/ServerNetworker.h"
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

namespace
{
    const std::string    defaultHost     = "127.0.0.1";
    const std::string    defaultProfile  = "Supervisor";
    const std::string    defaultPassword = "";
    const unsigned int   timeOut         = 1000;

    struct Timeout
    {
        explicit Timeout( unsigned int duration ) : duration_( duration ) { Start(); }
        void Start()
        {
            start_ = boost::posix_time::microsec_clock::universal_time();
        }
        bool Expired() const
        {
            return ( boost::posix_time::microsec_clock::universal_time() - start_ ).total_milliseconds() > duration_;
        }
        unsigned int duration_;
        boost::posix_time::ptime start_;
    };
}

namespace
{
    MOCK_BASE_CLASS( MockServer, tools::ServerNetworker )
    {
    public:
        explicit MockServer( unsigned short port )
            : tools::ServerNetworker( port )
        {
            RegisterMessage( *this, &MockServer::ReceiveClientToAuthentication );
            RegisterMessage( *this, &MockServer::ReceiveClientToSim );
            RegisterMessage( *this, &MockServer::ReceiveClientToMessenger );
            RegisterMessage( *this, &MockServer::ReceiveClientToAar );
            RegisterMessage( *this, &MockServer::ReceiveClientToReplay );
            AllowConnections();
        }
        void ReceiveClientToSim( const std::string& /*endpoint*/, const sword::ClientToSim& /*message*/ ) {}
        void ReceiveClientToAar( const std::string& /*endpoint*/, const sword::ClientToAar& /*message*/ ) {}
        void ReceiveClientToReplay( const std::string& /*endpoint*/, const sword::ClientToReplay& /*message*/ ) {}
        void ReceiveClientToMessenger( const std::string& /*endpoint*/, const sword::ClientToMessenger& /*message*/ ) {}

        void ReceiveClientToAuthentication( const std::string& /*endpoint*/, const sword::ClientToAuthentication& /*message*/ )
        {
            sword::AuthenticationToClient container;
            container.set_context( 0 );
            sword::AuthenticationResponse& response = *container.mutable_message()->mutable_authentication_response();
            response.set_error_code( sword::AuthenticationResponse::success );
            response.mutable_server_version()->set_value( sword::ProtocolVersion().value() );
            response.mutable_profile()->set_login( defaultProfile );
            response.mutable_profile()->set_supervisor( true );
            tools::MessageSender_ABC::Send( client_, container );
            SendMessageStub< sword::SimToClient >();
            SendMessageStub< sword::MessengerToClient >();
        }
        virtual void ConnectionSucceeded( const std::string& endpoint )
        {
            client_ = endpoint;
            tools::ServerNetworker::ConnectionSucceeded( endpoint );
        }

    private:
        template< typename T >
        void SendMessageStub()
        {
            T container;
            container.set_context( 0 );
            container.mutable_message();
            tools::MessageSender_ABC::Send( client_, container );
        }

    private:
        std::string client_;
    };

    MOCK_BASE_CLASS( MockConnectionHandler, SwordConnectionHandler_ABC )
    {
        MOCK_METHOD( OnConnectionSucceeded, 1 );
        MOCK_METHOD( OnConnectionFailed, 2 );
        MOCK_METHOD( OnConnectionError, 2 );
        MOCK_METHOD( OnConnectionWarning, 2 );
        MOCK_METHOD( OnAuthenticationSucceeded, 1 );
        MOCK_METHOD( OnAuthenticationFailed, 2 );
    };

    MOCK_BASE_CLASS( MockMessageHandler, SwordMessageHandler_ABC )
    {
        MOCK_METHOD_EXT( OnReceiveMessage, 1, void ( const sword::SimToClient& ), OnReceiveMessageSimToClient );
        MOCK_METHOD_EXT( OnReceiveMessage, 1, void ( const sword::MessengerToClient& ), OnReceiveMessageMessengerToClient );
        MOCK_METHOD_EXT( OnReceiveMessage, 1, void ( const sword::AuthenticationToClient& ), OnReceiveMessageAuthenticationToClient );
        MOCK_METHOD_EXT( OnReceiveMessage, 1, void ( const sword::DispatcherToClient& ), OnReceiveMessageDispatcherToClient );
    };

    std::string MakeHost( const std::string& host, unsigned short port )
    {
        return host + ":" + boost::lexical_cast< std::string >( port );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientConnectsAndAuthenticatesToSimulation
// Created: SBO 2010-10-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientConnectsAndAuthenticatesToSimulation )
{
    MockServer server( PORT );
    SwordProxy client( defaultHost, PORT, defaultProfile, defaultPassword );
    MockConnectionHandler connectionHandler;
    MOCK_EXPECT( connectionHandler, OnConnectionSucceeded ).once().with( MakeHost( defaultHost, PORT ) );
    MOCK_EXPECT( connectionHandler, OnAuthenticationSucceeded ).once().with( defaultProfile );
    client.Connect( &connectionHandler );
    Timeout timeout( timeOut );
    while( !timeout.Expired() )
    {
        server.Update();
        client.Update();
    }
    client.Disconnect();
}

// -----------------------------------------------------------------------------
// Name: MessageHandlersCanBeRegistered
// Created: SBO 2010-10-11
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( MessageHandlersCanBeRegistered )
{
    MockServer server( PORT );
    SwordProxy client( defaultHost, PORT, defaultProfile, defaultPassword );
    MockConnectionHandler connectionHandler;
    MOCK_EXPECT( connectionHandler, OnConnectionSucceeded ).once().with( MakeHost( defaultHost, PORT ) );
    MOCK_EXPECT( connectionHandler, OnAuthenticationSucceeded ).once().with( defaultProfile );
    MockMessageHandler messageHandler;
    MOCK_EXPECT( messageHandler, OnReceiveMessageSimToClient ).at_least( 1 );
    MOCK_EXPECT( messageHandler, OnReceiveMessageMessengerToClient ).at_least( 1 );
    MOCK_EXPECT( messageHandler, OnReceiveMessageAuthenticationToClient ).at_least( 1 );
    client.RegisterMessageHandler( &messageHandler );
    client.Connect( &connectionHandler );
    Timeout timeout( timeOut );
    while( !timeout.Expired() )
    {
        server.Update();
        client.Update();
    }
    client.UnregisterMessageHandler( &messageHandler );
    client.Disconnect();
}
