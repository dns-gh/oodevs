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
#include "tools/ClientNetworker.h"
#include "tools/ServerNetworker.h"
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>
#include <windows.h>

using namespace mockpp;
using namespace google::protobuf;

namespace
{
    std::string LOCALHOST = "127.0.0.1:10000";
}

namespace
{
    struct MessageSendingFixture
    {
        MessageSendingFixture()
        {
            server_.ConnectionSucceeded_mocker.expects( once() ).with( mockpp::any() );
            client_.ConnectionSucceeded_mocker.expects( once() ).with( eq< const std::string >( LOCALHOST ) );
            while( !client_.Connected() )
            {
                BOOST_CHECK_NO_THROW( client_.Update() );
                BOOST_CHECK_NO_THROW( server_.Update() );
            }
        }
        template< typename M >
        void VerifyServerReception( M& message, unsigned int count = 1 )
        {
            AddServerExpectation( message, count );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                BOOST_CHECK_NO_THROW( client_.Send( LOCALHOST, message ) );
                server_.ResetReceived();
                while( !server_.Received() )
                {
                    BOOST_CHECK_NO_THROW( client_.Update() );
                    BOOST_CHECK_NO_THROW( server_.Update() );
                }
            }
            std::cout << "sent " << count << " message(s) in " 
                      << boost::posix_time::microsec_clock::universal_time() - start << std::endl;
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
                BOOST_CHECK_NO_THROW( server_.Send( message ) );
                client_.ResetReceived();
                while( !client_.Received() )
                {
                    BOOST_CHECK_NO_THROW( server_.Update() );
                    BOOST_CHECK_NO_THROW( client_.Update() );
                }
            }
            std::cout << "sent " << count << " message(s) in " 
                      << boost::posix_time::microsec_clock::universal_time() - start << std::endl;
            client_.verify();
            server_.verify();
        }

        void AddClientExpectation( MsgPion& message, unsigned int count = 1 )
        {
            client_.OnReceivePion_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const MsgPion >( message ) );
        }
        
        void AddClientExpectation( Test& message, unsigned int count = 1 )
        {
            client_.OnReceiveTest_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const Test >( message ) );
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
    message.set_name( "Pion name\n" );
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

    Test test;
    test.set_name( "My test" );
    VerifyClientReception( test );
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
