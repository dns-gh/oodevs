// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "shield/Server.h"
#include "shield/Listener_ABC.h"
#pragma warning( push, 0 )
#include "shield/proto/ClientToSim.pb.h"
#include "shield/proto/SimToClient.pb.h"
#pragma warning( pop )
#include "tools/ServerNetworker.h"
#include "tools/ClientNetworker.h"
#include "protocol/Simulation.h"
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

namespace bl = boost::lambda;

namespace
{
    MOCK_BASE_CLASS( MockListener, shield::Listener_ABC )
    {
        MOCK_METHOD( Info, 1 )
        MOCK_METHOD( Error, 1 )
        MOCK_METHOD( Debug, 1 )
    };

    template< typename C, typename F >
    void wait( C condition, F flush, int timeout = 10000, int sleep = 100 )
    {
        while( !condition() && timeout > 0 )
        {
            --timeout;
            if( sleep > 0 )
                boost::this_thread::sleep( boost::posix_time::milliseconds( sleep ) );
            flush();
        }
    }

    struct Client : tools::ClientNetworker
    {
        Client()
            : tools::ClientNetworker( "localhost:" + boost::lexical_cast< std::string >( PORT ) )
        {
            RegisterMessage( *this, &Client::Receive );
        }
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const MsgsSimToClient::MsgSimToClient& ), Receive );
        MOCK_METHOD_EXT( ConnectionSucceeded, 1, void( const std::string& ), ConnectionSucceeded );
        MOCK_METHOD_EXT( ConnectionFailed, 2, void( const std::string&, const std::string& ), ConnectionFailed );
        MOCK_METHOD_EXT( ConnectionError, 2, void( const std::string&, const std::string& ), ConnectionError );
        std::string host;
    };
    struct Server : tools::ServerNetworker
    {
        Server()
            : tools::ServerNetworker( PORT + 1 )
        {
            AllowConnections();
            RegisterMessage( *this, &Server::Receive );
        }
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const sword::ClientToSim& ), Receive );
        MOCK_METHOD_EXT( ConnectionSucceeded, 1, void( const std::string& ), ConnectionSucceeded );
        MOCK_METHOD_EXT( ConnectionFailed, 2, void( const std::string&, const std::string& ), ConnectionFailed );
        MOCK_METHOD_EXT( ConnectionError, 2, void( const std::string&, const std::string& ), ConnectionError );
        std::string host;
    };
    struct ListenerFixture
    {
        ListenerFixture()
        {
            MOCK_EXPECT( listener, Info ).once().with( mock::contain( "Starting shield server on port" ) );
        }
        MockListener listener;
    };
    struct Fixture : ListenerFixture
    {
        Fixture()
            : shield( PORT, "localhost:" + boost::lexical_cast< std::string >( PORT + 1 ), listener, true )
        {
            MOCK_EXPECT( listener, Info ).once().with( mock::contain( "Shield proxy received connection from" ) );
            MOCK_EXPECT( listener, Info ).once().with( mock::contain( "Shield proxy connected to" ) );
            int connected = 0;
            MOCK_EXPECT( server, ConnectionSucceeded ).once().with( mock::retrieve( server.host ) ).calls( ++bl::var( connected ) );
            MOCK_EXPECT( client, ConnectionSucceeded ).once().with( mock::retrieve( client.host ) ).calls( ++bl::var( connected ) );
            wait( bl::var( connected ) == 2, boost::bind( &Fixture::Update, this ) );
            mock::verify();
            mock::reset();
        }
        void Update()
        {
            shield.Update();
            server.Update();
            client.Update();
        }
        shield::Server shield;
        Server server;
        Client client;
    };
}

BOOST_AUTO_TEST_CASE( non_existing_dispatcher_is_detected_by_proxy_when_a_client_connects )
{
    MockListener listener;
    MOCK_EXPECT( listener, Info ).once().with( mock::contain( "Starting shield server on port" ) );
    shield::Server shield( PORT, "localhost:" + boost::lexical_cast< std::string >( PORT + 1 ), listener, true );
    Client client;
    MOCK_EXPECT( listener, Info ).once().with( mock::contain( "Shield proxy received connection from" ) );
    int errors = 3;
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection to" ) &&
                                                mock::contain( "failed" ) ).calls( --bl::var( errors ) );
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection from" ) &&
                                                mock::contain( "aborted" ) ).calls( --bl::var( errors ) );
    MOCK_EXPECT( client, ConnectionSucceeded ).once();
    MOCK_EXPECT( client, ConnectionError ).once().calls( --bl::var( errors ) );
    wait( bl::var( errors ) == 0, (bl::bind( &shield::Server::Update, &shield ), bl::bind( &Client::Update, &client )) );
}

BOOST_FIXTURE_TEST_CASE( message_sent_from_client_is_received_on_simulation, Fixture )
{
    {
        MsgsClientToSim::MsgClientToSim msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_control_checkpoint_set_frequency()->set_frequency( 12 );
        MOCK_EXPECT( listener, Debug ).once();
        client.Send( client.host, msg );
    }
    bool received = false;
    MOCK_EXPECT( listener, Debug ).once();
    MOCK_EXPECT( server, Receive ).once().calls( bl::var( received ) = true );
    wait( bl::var( received ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( unsupported_message_sent_from_client_is_detected_by_proxy, Fixture )
{
    {
        sword::SimToClient msg;
        msg.mutable_message();
        client.Send( client.host, msg );
    }
    int notified = 3;
    MOCK_EXPECT( server, ConnectionError ).once().calls( --bl::var( notified ) );
    MOCK_EXPECT( client, ConnectionError ).once().calls( --bl::var( notified ) );
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection from" ) &&
                                                mock::contain( "aborted" ) &&
                                                mock::contain( "Unknown message tag" ) ).calls( --bl::var( notified ) );
    MOCK_EXPECT( listener, Error );
    wait( bl::var( notified ) == 0, boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( message_sent_from_simulation_is_received_on_client, Fixture )
{
    {
        sword::SimToClient msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_unit_creation_request_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
        server.Send( server.host, msg );
    }
    bool received = false;
    MOCK_EXPECT( client, Receive ).once().calls( bl::var( received ) = true );
    MOCK_EXPECT( listener, Debug ).once();
    wait( bl::var( received ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( unsupported_message_sent_from_simulation_is_detected_by_proxy, Fixture )
{
    {
        sword::ClientToSim msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_control_checkpoint_set_frequency()->set_frequency( 12 );
        server.Send( server.host, msg );
    }
    int notified = 3;
    MOCK_EXPECT( server, ConnectionError ).once().calls( --bl::var( notified ) );
    MOCK_EXPECT( client, ConnectionError ).once().calls( --bl::var( notified ) );
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection to" ) &&
                                                mock::contain( "aborted" ) &&
                                                mock::contain( "Unknown message tag" ) ).calls( --bl::var( notified ) );
    MOCK_EXPECT( listener, Error );
    wait( bl::var( notified ) == 0, boost::bind( &Fixture::Update, this ) );
}
