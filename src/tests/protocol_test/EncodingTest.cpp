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
#include "tools/ClientNetworker.h"
#include "tools/ServerNetworker.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

using namespace mockpp;
using namespace google::protobuf;

namespace
{
    bool operator==( const SimpleTest& lhs, const SimpleTest& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.name(), rhs.name() );
        return lhs.name() == rhs.name();
    }

    bool operator==( const ZeroField& lhs, const ZeroField& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.value(), rhs.value() );
        return lhs.value() == rhs.value();
    }

    bool operator==( const Composite& lhs, const Composite& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.has_context(), rhs.has_context() );
        if( lhs.has_context() )
            BOOST_CHECK_EQUAL( lhs.context(), rhs.context() );
        BOOST_CHECK_EQUAL( lhs.has_simple(), rhs.has_simple() );
        if( lhs.has_simple() )
            BOOST_CHECK_EQUAL( lhs.simple().name(), rhs.simple().name() );
        BOOST_CHECK_EQUAL( lhs.has_zero(), rhs.has_zero() );
        if( lhs.has_zero() )
            BOOST_CHECK_EQUAL( lhs.zero().value(), rhs.zero().value() );
        return true;
    }

    bool operator==( const Extendable& lhs, const Extendable& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.HasExtension( simple ), rhs.HasExtension( simple ) );
        return true;
    }

    struct EncodingFixture
    {
        template< typename M >
        void Verify( M& message, unsigned int count = 1 )
        {
            BOOST_REQUIRE( message.IsInitialized() );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                std::auto_ptr< tools::MessageEncoder< M > > encoder;
                BOOST_REQUIRE_NO_THROW( encoder.reset( new tools::MessageEncoder< M >( message ) ) );
                const tools::Message& encodedMessage = *encoder;
                tools::Message encodedCopy = encodedMessage;
                std::auto_ptr< tools::MessageDecoder< M > > decoder;
                BOOST_REQUIRE_NO_THROW( decoder.reset( new tools::MessageDecoder< M >( StripHeader( encodedCopy ) ) ) );
                const M& decodedMessage = *decoder;
                BOOST_CHECK( message == decodedMessage );
                std::cout << "- Original size: " << sizeof( message ) << std::endl
                          << "- Encoded size: " << encodedMessage.Size() - 2 * sizeof( unsigned long ) << std::endl;
            }
            std::cout << "Encoded " << count << " message(s) in " 
                      << boost::posix_time::microsec_clock::universal_time() - start << std::endl;
        }

        tools::Message& StripHeader( tools::Message& message )
        {
            unsigned long size, tag;
            message >> size >> tag;
            return message;
        }
    };
}

BOOST_FIXTURE_TEST_SUITE( EncodingTestSuite, EncodingFixture )

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeSimpleTest
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeSimpleTest )
{
    SimpleTest message;
    message.set_name( "My name" );
    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeCompositeStringTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeCompositeStringTest )
{
    const unsigned int count = 1024;
    CompositeStringTest message;
    for( unsigned int i = 0; i < count; ++i )
        message.add_test()->set_name( "My name" );
    Verify( message, 1 );
//    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeCompositeIntegerTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeCompositeIntegerTest )
{
    const unsigned int count = 1024;
    CompositeIntegerTest message;
    for( unsigned int i = 0; i < count; ++i )
        message.add_test( 42 );
    Verify( message, 1 );
//    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeZeroField
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeZeroField )
{
    ZeroField message;
    Verify( message );
    message.set_value( 1 );
    Verify( message );
    message.set_value( 5 );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeComposite
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeComposite )
{
    Composite message;
    message.set_context( 0 );
    Verify( message );
    message.set_context( 5 );
    message.mutable_simple()->set_name( "My name" );
    Verify( message );
    message.Clear();
    message.set_context( 1 );
    message.mutable_zero()->set_value( 0 );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeExtendable
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeExtendable )
{
    Extendable message;
    Verify( message );
    message.MutableExtension( simple )->set_name( "My name" );
    Verify( message );
    message.MutableExtension( zero )->set_value( 0 );
    Verify( message );
}

BOOST_AUTO_TEST_SUITE_END()

// -----------------------------------------------------------------------------
// Name: ProtobufTest_ErrorChecking
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_ErrorChecking )
{
    Composite message;
    std::auto_ptr< tools::MessageEncoder< Composite > > encoder;
    BOOST_CHECK_THROW( encoder.reset( new tools::MessageEncoder< Composite >( message ) ), std::runtime_error );
}
