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
#pragma warning( push, 0 )
#include <boost/date_time/gregorian/gregorian.hpp>
#pragma warning( pop )

#define BOOST_FAIL_ON_DATE( year, month, day )                                                                  \
    const boost::gregorian::date expected( year, month, day );                                                  \
    const boost::gregorian::date actual( boost::gregorian::day_clock::local_day() );                            \
    if( actual >= expected )                                                                                    \
        BOOST_FAIL( "expected date '" + boost::gregorian::to_simple_string( expected ) + "' has been reached" );

namespace
{
    struct Timeout : private boost::noncopyable
    {
        explicit Timeout( unsigned int duration )
            : duration_( duration )
        {
            Start();
        }
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
            : endpoint_( "127.0.0.1:33333" )
            , port_    ( 33333 )
            , client_  ( endpoint_ )
            , server_  ( port_ )
        {
            MOCK_EXPECT( server_.OnConnectionSucceeded, _ ).once();
            MOCK_EXPECT( client_.OnConnectionSucceeded, _ ).once().with( endpoint_ );
            Timeout timeout( 1000 );
            while( !client_.Connected() && !timeout.Expired() )
            {
                client_.Update();
                server_.Update();
            }
            BOOST_REQUIRE( client_.Connected() );
            mock::verify();
        }
        template< typename M >
        void VerifyServerReception( M& message, unsigned int count = 1 )
        {
            MOCK_EXPECT( server_.OnReceivePion, _ ).exactly( count ).with( mock::any, message );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                server_.ResetReceived();
                client_.Send( endpoint_, message );
                client_.Update();
                Timeout timeout( 100 );
                while( !server_.Received() && !timeout.Expired() )
                    server_.Update();
            }
            BOOST_TEST_MESSAGE( "sent " << count << " message(s) in " << boost::posix_time::microsec_clock::universal_time() - start );
            mock::verify();
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
                Timeout timeout( 100 );
                while( !client_.Received() && !timeout.Expired() )
                {
                    server_.Update();
                    client_.Update();
                }
            }
            BOOST_TEST_MESSAGE( "Sent " << count << " message(s) in " << boost::posix_time::microsec_clock::universal_time() - start );
            mock::verify();
        }

        void AddClientExpectation( MsgPion& message, unsigned int count = 1 )
        {
            MOCK_EXPECT( client_.OnReceivePion, _ ).exactly( count ).with( endpoint_, message );
        }

        void AddClientExpectation( EmptyMessage& message, unsigned int count = 1 )
        {
            MOCK_EXPECT( client_.OnReceiveEmpty, _ ).exactly( count ).with( endpoint_, message );
        }

        const std::string endpoint_;
        const unsigned short port_;
        MockClient client_;
        MockServer server_;
    };
}


//BOOST_FIXTURE_TEST_SUITE( MessageTestSuite, MessageSendingFixture )

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendOneMessageFromClientToServer
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendOneMessageFromClientToServer )
{
    BOOST_FAIL_ON_DATE( 2011, 02, 01 );
    //MsgPion message;
    //message.set_id( 101 );
    //message.set_name( "My name" );
    //VerifyServerReception( message );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendMultipleMessageToServer
// Created: SBO 2009-10-26
// $$$$ SBO 2009-10-26: mainly tests network interface performance
//                      see below for encoding performance tests
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendMultipleMessageToServer )
{
    BOOST_FAIL_ON_DATE( 2011, 02, 01 );
    //MsgPion message;
    //message.set_id( 101 );
    //message.set_name( "test" );
    //VerifyServerReception( message, 10 );
    //VerifyServerReception( message, 100 );
    //VerifyServerReception( message, 1000 );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendMessageBothWays
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendMessageBothWays )
{
    BOOST_FAIL_ON_DATE( 2011, 02, 01 );
    //MsgPion message;
    //message.set_id( 1 );
    //message.set_name( "My name" );
    //VerifyClientReception( message );
    //VerifyServerReception( message );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendEmptyMessage
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendEmptyMessage )
{
    BOOST_FAIL_ON_DATE( 2011, 02, 01 );
    //EmptyMessage message;
    //VerifyClientReception( message );
}

//BOOST_AUTO_TEST_SUITE_END()
