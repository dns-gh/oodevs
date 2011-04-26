// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "protocol_test_pch.h"
#include "Protocol.h"
#include "MessageHelpers.h"
#include "MockClient.h"
#include "MockServer.h"
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>

namespace
{
    template< typename F >
    void wait( bool& condition, F flush, int timeout = 1000, int sleep = 100 )
    {
        while( !condition && timeout > 0 )
        {
            --timeout;
            if( sleep > 0 )
                boost::this_thread::sleep( boost::posix_time::milliseconds( sleep ) );
            flush();
        }
    }

    struct Fixture
    {
        Fixture()
            : endpoint_( "127.0.0.1:33333" )
            , client_  ( endpoint_ )
            , server_  ( 33333 )
        {
            bool connected = false;
            MOCK_EXPECT( server_, ConnectionSucceeded ).once().with( mock::retrieve( link_ ) );
            MOCK_EXPECT( client_, ConnectionSucceeded ).once().with( endpoint_ ).calls( boost::lambda::var( connected ) = true );
            wait( connected, boost::bind( &Fixture::Update, this ) );
            mock::verify();
            mock::reset();
        }

        template< typename From, typename Message, typename Expectation >
        void VerifyReception( From& from, const std::string& endpoint, const Message& message, Expectation& expectation ) // $$$$ MCO : expectation should be copiable but isn't due to a bug in turtle
        {
            from.Send( endpoint, message );
            bool received = false;
            expectation.once().with( mock::any, message ).calls( boost::lambda::var( received ) = true );
            wait( received, boost::bind( &Fixture::Update, this ) );
        }

        void VerifyServerReception( const MsgPion& message )
        {
            VerifyReception( client_, endpoint_, message, MOCK_EXPECT( server_, OnReceivePion ) );
        }

        void VerifyClientReception( const MsgPion& message )
        {
            VerifyReception( server_, link_, message, MOCK_EXPECT( client_, OnReceivePion ) );
        }

        void VerifyClientReception( const EmptyMessage& message )
        {
            VerifyReception( server_, link_, message, MOCK_EXPECT( client_, OnReceiveEmpty ) );
        }

        void Update()
        {
            client_.Update();
            server_.Update();
        }

        std::string endpoint_;
        std::string link_;
        MockClient client_;
        MockServer server_;
    };
}

BOOST_FIXTURE_TEST_CASE( a_message_sent_from_client_is_received_on_server, Fixture )
{
    MsgPion message;
    message.set_id( 101 );
    message.set_name( "My name" );
    VerifyServerReception( message );
}

BOOST_FIXTURE_TEST_CASE( a_message_sent_both_ways_is_received_on_each_other_side, Fixture )
{
    MsgPion message;
    message.set_id( 1 );
    message.set_name( "My name" );
    VerifyClientReception( message );
    VerifyServerReception( message );
}

BOOST_FIXTURE_TEST_CASE( an_empty_message_sent_is_received_on_the_other_side, Fixture )
{
    EmptyMessage message;
    VerifyClientReception( message );
}
