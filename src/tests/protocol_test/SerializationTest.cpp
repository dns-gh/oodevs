// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "protocol_test_pch.h"
#include "protocol_includes.h"
#include "MessageHelpers.h"
#include "MockClient.h"
#include "MockServer.h"

using namespace mockpp;

namespace
{
    const std::string LOCALHOST = "127.0.0.1:10000";

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
        const unsigned int duration_;
        boost::posix_time::ptime start_;
    };

    struct MessageSendingFixture
    {
        MessageSendingFixture()
        {
            server_.ConnectionSucceeded_mocker.expects( once() ).with( mockpp::any() );
            client_.ConnectionSucceeded_mocker.expects( once() ).with( eq< const std::string >( LOCALHOST ) );
            Timeout timeout( 100 );
            while( !client_.Connected() && !timeout.Expired() )
            {
                client_.Update();
                server_.Update();
            }
        }
        template< typename M >
        void VerifyServerReception( M& message, unsigned int count = 1 )
        {
            AddServerExpectation( message, count );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                server_.ResetReceived();
                client_.Send( LOCALHOST, message );
                client_.Update();
                Timeout timeout( 100 );
                while( !server_.Received() && !timeout.Expired() )
                    server_.Update();
            }
            BOOST_TEST_MESSAGE( "sent " << count << " message(s) in " << boost::posix_time::microsec_clock::universal_time() - start );
            client_.verify();
            server_.verify();
        }

        void AddServerExpectation( MsgPion& message, unsigned int count = 1 )
        {
            server_.OnReceivePion_mocker.expects( exactly( count ) ).with( mockpp::any(), eq< const MsgPion >( message ) );
        }

        template< typename M >
        void VerifyClientReception( M& message, unsigned int count = 1 )
        {
            AddClientExpectation( message, count );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                client_.ResetReceived();
                server_.Send( message );
                server_.Update();
                Timeout timeout( 100 );
                while( !client_.Received() && !timeout.Expired() )
                    client_.Update();
            }
            BOOST_TEST_MESSAGE( "Sent " << count << " message(s) in " << boost::posix_time::microsec_clock::universal_time() - start );
            client_.verify();
            server_.verify();
        }

        void AddClientExpectation( MsgPion& message, unsigned int count = 1 )
        {
            client_.OnReceivePion_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const MsgPion >( message ) );
        }
        
        void AddClientExpectation( EmptyMessage& message, unsigned int count = 1 )
        {
            client_.OnReceiveEmpty_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const EmptyMessage >( message ) );
        }

        MockClient client_;
        MockServer server_;
    };
}

BOOST_FIXTURE_TEST_SUITE( MessageTestSuite, MessageSendingFixture )

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendOneMessageFromClientToServer
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendOneMessageFromClientToServer )
{
    MsgPion message;
    message.set_id( 101 );
    message.set_name( "My name" );
    VerifyServerReception( message );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendMultipleMessageToServer
// Created: SBO 2009-10-26
// $$$$ SBO 2009-10-26: mainly tests network interface performance
//                      see below for encoding performance tests
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendMultipleMessageToServer )
{
    MsgPion message;
    message.set_id( 101 );
    message.set_name( "test" );
    VerifyServerReception( message, 10 );
//    VerifyServerReception( message, 100 );
//    VerifyServerReception( message, 1000 );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendMessageBothWays
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendMessageBothWays )
{
    MsgPion message;
    message.set_id( 1 );
    message.set_name( "My name" );
    VerifyClientReception( message );
    VerifyServerReception( message );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendEmptyMessage
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendEmptyMessage )
{
    EmptyMessage message;
    VerifyClientReception( message );
}

BOOST_AUTO_TEST_SUITE_END()
