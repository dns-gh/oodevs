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
#include "tools/asio.h"
#include "tools/Message.h"
#include "tools/MessageIdentifierFactory.h"
#include <boost/shared_array.hpp>

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

    tools::Message& StripHeader( tools::Message& message )
    {
        unsigned long size, tag;
        message >> size >> tag;
        BOOST_CHECK_GE( size, sizeof( tag ) );
        return message;
    }
    template< typename M >
    void CheckOutputBuffer( M& message )
    {
        std::vector< unsigned long > expected;
        expected.push_back( static_cast< unsigned long >( message.Size() ) - sizeof( unsigned long ) );
        const unsigned long tag = tools::MessageIdentifierFactory::GetIdentifier< M >();
        expected.push_back( tag );
        boost::asio::const_buffers_1 buffer = message.MakeOutputBuffer( tag );
        std::vector< unsigned long >::const_iterator expectedIt = expected.begin();
        for( boost::asio::const_buffers_1::const_iterator it = buffer.begin(); expectedIt != expected.end() && it != buffer.end(); ++it, ++expectedIt )
            BOOST_CHECK_EQUAL( *expectedIt, ntohl( *boost::asio::buffer_cast< const unsigned long* >( *it ) ) );
        // $$$$ MCO : what if one of the messages/buffers has more elements than the other ?
    }
    template< typename M >
    void Verify( const M& message )
    {
        BOOST_REQUIRE( message.IsInitialized() );
        boost::shared_array< google::protobuf::uint8 > buffer( new google::protobuf::uint8[ message.ByteSize() ] );
        BOOST_REQUIRE( message.SerializeWithCachedSizesToArray( buffer.get() ) );
        tools::Message encodedMessage;
        encodedMessage.Write( (const char*)buffer.get(), message.GetCachedSize() );
        CheckOutputBuffer( encodedMessage );
        tools::Message encodedCopy = encodedMessage;
        tools::Message stripped = StripHeader( encodedCopy );
        M decodedMessage;
        BOOST_REQUIRE( decodedMessage.ParseFromArray( stripped.Data(), static_cast< int >( stripped.Size() ) ) );
        BOOST_CHECK( message == decodedMessage );
        static const unsigned long headerSize = 2 * sizeof( unsigned long );
        BOOST_CHECK_EQUAL( static_cast< unsigned long >( message.ByteSize() ), encodedMessage.Size() - headerSize );
    }
}

// -----------------------------------------------------------------------------
// Name: EncodingTest_EncodeEmptyMessage
// Created: SBO 2011-01-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeEmptyMessage )
{
    EmptyMessage message;
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: EncodingTest_EncodeSimpleTest
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeSimpleTest )
{
    SimpleTest message;
    message.set_name( "My name" );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: EncodingTest_EncodeCompositeStringTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeCompositeStringTest )
{
    const unsigned int count = 1024;
    CompositeStringTest message;
    for( unsigned int i = 0; i < count; ++i )
        message.add_test()->set_name( "My name" );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: EncodingTest_EncodeCompositeIntegerTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeCompositeIntegerTest )
{
    const unsigned int count = 1024;
    CompositeIntegerTest message;
    for( unsigned int i = 0; i < count; ++i )
        message.add_test( 42 );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: EncodingTest_EncodeZeroField
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeZeroField )
{
    ZeroField message;
    Verify( message );
    message.set_value( 1 );
    Verify( message );
    message.set_value( 5 );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: EncodingTest_EncodeComposite
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeComposite )
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
// Name: EncodingTest_EncodeExtendable
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EncodingTest_EncodeExtendable )
{
    Extendable message;
    Verify( message );
    message.MutableExtension( simple )->set_name( "My name" );
    Verify( message );
    message.MutableExtension( zero )->set_value( 0 );
    Verify( message );
}
