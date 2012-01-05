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
#include "shield/DebugInfo_ABC.h"
#include "shield/ClientHandler_ABC.h"
#include "shield/Model_ABC.h"
#pragma warning( push, 0 )
#include "shield/proto/ClientToSim.pb.h"
#include "shield/proto/SimToClient.pb.h"
#include "shield/proto/AdminToLauncher.pb.h"
#pragma warning( pop )
#include "tools/ServerNetworker.h"
#include "tools/ClientNetworker.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Simulation.h"
#include "protocol/Launcher.h"
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
    MOCK_BASE_CLASS( MockMessageDispatcher, tools::MessageDispatcher_ABC )
    {
        MOCK_METHOD( GetNbMessagesReceived, 0 )
        MOCK_METHOD( Register, 2 )
        MOCK_METHOD( Retrieve, 1 )
    };
    MOCK_BASE_CLASS( MockClientHandler, shield::ClientHandler_ABC )
    {
        MOCK_METHOD( Register, 3 )
        MOCK_METHOD( Unregister, 1 )
    };
    MOCK_BASE_CLASS( MockModel, shield::Model_ABC )
    {
        MOCK_METHOD( Send, 1 )
    };

    template< typename C, typename F >
    void wait( C condition, F flush, int timeout = 100, int sleep = 100 )
    {
        while( !condition() && timeout > 0 )
        {
            --timeout;
            if( sleep > 0 )
                boost::this_thread::sleep( boost::posix_time::milliseconds( sleep ) );
            flush();
        }
    }

    MOCK_BASE_CLASS( Client, tools::ClientNetworker )
    {
        Client()
            : tools::ClientNetworker( "localhost:" + boost::lexical_cast< std::string >( PORT ) )
        {
            RegisterMessage( *this, &Client::Receive );
        }
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const MsgsSimToClient::MsgSimToClient& ), Receive );
        MOCK_METHOD( ConnectionSucceeded, 1 );
        MOCK_METHOD( ConnectionFailed, 2 );
        MOCK_METHOD( ConnectionError, 2 );
        MOCK_METHOD( ConnectionWarning, 2 );
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
            : shield( PORT, dispatcher, model, handler, listener, true )
        {
            int notified = 4;
			shield.AllowConnections();
            MOCK_EXPECT( listener, Info ).once().with( mock::contain( "Shield proxy received connection from" ) ).calls( --bl::var( notified ) );
            MOCK_EXPECT( handler, Register ).once().with( mock::any, mock::retrieve( sender ), mock::retrieve( broadcaster ) )
                .calls( (--bl::var( notified ), bl::bind( &dispatcher::ClientBroadcaster_ABC::Activate, &bl::_3, bl::_1 )) );
            MOCK_EXPECT( client, ConnectionSucceeded ).once().with( mock::retrieve( client.host ) ).calls( --bl::var( notified ) );
            MOCK_EXPECT( model, Send ).once().calls( --bl::var( notified ) );
            wait( bl::var( notified ) == 0, boost::bind( &Fixture::Update, this ) );
            mock::verify();
            mock::reset();
            MOCK_EXPECT( handler, Unregister ).once();
        }
        void Update()
        {
            shield.Update();
            client.Update();
        }
        MockMessageDispatcher dispatcher;
        MockClientHandler handler;
        MockModel model;
        shield::Server shield;
        Client client;
        tools::MessageSender_ABC* sender;
        dispatcher::ClientBroadcaster_ABC* broadcaster;
    };

    struct start_with
    {
        start_with( const std::string& s )
            : s_( s )
        {}
        template< typename T >
        friend bool operator==( const T& lhs, const start_with& rhs )
        {
            return boost::lexical_cast< std::string >( lhs ).find( rhs.s_ ) == 0;
        }
        std::string s_;
    };
}

BOOST_FIXTURE_TEST_CASE( message_sent_from_client_is_received_on_simulation, Fixture )
{
    MOCK_FUNCTOR( void( const std::string&, const sword::ClientToSim& ) ) functor;
    tools::ObjectMessageCallback< sword::ClientToSim > callback;
    callback.AddCallback( functor );
    MOCK_EXPECT( dispatcher, Retrieve ).returns( &callback );
    {
        MsgsClientToSim::MsgClientToSim msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_control_checkpoint_set_frequency()->set_frequency( 12 );
        MOCK_EXPECT( listener, Debug ).once().with( start_with( "Shield received" ) );
        client.Send( client.host, msg );
    }
    bool received = false;
    MOCK_EXPECT( functor, _ ).once().calls( bl::var( received ) = true );
    wait( bl::var( received ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( message_sent_from_admin_is_received_on_launcher, Fixture )
{
    MOCK_FUNCTOR( void( const std::string&, const sword::AdminToLauncher& ) ) functor;
    tools::ObjectMessageCallback< sword::AdminToLauncher > callback;
    callback.AddCallback( functor );
    MOCK_EXPECT( dispatcher, Retrieve ).returns( &callback );
    {
        MsgsAdminToLauncher::MsgAdminToLauncher msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_exercise_list_request();
        MOCK_EXPECT( listener, Debug ).once().with( start_with( "Shield received" ) );
        client.Send( client.host, msg );
    }
    bool received = false;
    MOCK_EXPECT( functor, _ ).once().calls( bl::var( received ) = true );
    wait( bl::var( received ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( unsupported_message_sent_from_client_is_detected_by_proxy, Fixture )
{
    {
        sword::SimToClient msg;
        msg.mutable_message();
        client.Send( client.host, msg );
    }
    bool notified = false;
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection from" ) &&
                                                mock::contain( "warning" ) &&
                                                mock::contain( "Unknown message tag" ) ).calls( bl::var( notified ) = true );
    wait( bl::var( notified ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( message_a_bit_long_sent_from_client_is_detected_by_proxy, Fixture )
{
    MOCK_FUNCTOR( void( const std::string&, const sword::ClientToSim& ) ) functor;
    tools::ObjectMessageCallback< sword::ClientToSim > callback;
    callback.AddCallback( functor );
    MOCK_EXPECT( dispatcher, Retrieve ).returns( &callback );
    {
        MsgsClientToSim::MsgClientToSim msg;
        msg.set_context( 77 );
        std::string name( 32 * 1024, 'a' ); // 32 kB
        msg.mutable_message()->mutable_control_checkpoint_save_now()->set_name( name );
        MOCK_EXPECT( listener, Debug ).once().with( start_with( "Shield received" ) );
        client.Send( client.host, msg );
    }
    int notified = 1;
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection from" ) &&
                                                mock::contain( "warning" ) &&
                                                mock::contain( "Message size warning" ) ).calls( --bl::var( notified ) );
    bool received = false;
    MOCK_EXPECT( functor, _ ).once().calls( bl::var( received ) = true );
    wait( bl::var( received ) && bl::var( notified ) == 0, boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( message_too_long_sent_from_client_is_detected_by_proxy, Fixture )
{
    {
        tools::Message msg( 1000 * 1024 + 12 ); // 1 MB + margin for header and stuff
        client.Send( client.host, 1, msg );
    }
    int notified = 2;
    MOCK_EXPECT( client, ConnectionError ).once().calls( --bl::var( notified ) );
    MOCK_EXPECT( client, ConnectionError );
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection from" ) &&
                                                mock::contain( "aborted" ) &&
                                                mock::contain( "Message size too large" ) ).calls( --bl::var( notified ) );
    MOCK_EXPECT( listener, Error );
    wait( bl::var( notified ) == 0, boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( message_sent_from_simulation_is_received_on_client, Fixture )
{
    {
        sword::SimToClient msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_unit_creation_request_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
        MOCK_EXPECT( listener, Debug ).once().with( start_with( "Shield sent" ) );
        sender->Send( "unused", msg );
    }
    bool received = false;
    MOCK_EXPECT( client, Receive ).once().calls( bl::var( received ) = true );
    wait( bl::var( received ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( message_broascasted_from_simulation_is_received_on_client, Fixture )
{
    {
        sword::SimToClient msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_unit_creation_request_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
        MOCK_EXPECT( listener, Debug ).once().with( start_with( "Shield sent" ) );
        broadcaster->Broadcast( msg );
    }
    bool received = false;
    MOCK_EXPECT( client, Receive ).once().calls( bl::var( received ) = true );
    wait( bl::var( received ), boost::bind( &Fixture::Update, this ) );
}

BOOST_FIXTURE_TEST_CASE( invalid_message_sent_from_client_is_logged_on_simulation, Fixture )
{
    MOCK_EXPECT( dispatcher, Retrieve ).returns( 0 );
    {
        MsgsClientToSim::MsgClientToSim msg;
        msg.set_context( 77 );
        msg.mutable_message()->mutable_control_checkpoint_set_frequency()->set_frequency( 12 );
        MOCK_EXPECT( listener, Debug ).once().with( start_with( "Shield received" ) );
        client.Send( client.host, msg );
    }
    bool notified = false;
    MOCK_EXPECT( listener, Error ).once().with( mock::contain( "Shield proxy connection from" ) &&
                                                mock::contain( "warning" ) &&
                                                mock::contain( "Unknown message" ) ).calls( bl::var( notified ) = true );
    wait( bl::var( notified ), boost::bind( &Fixture::Update, this ) );
}
