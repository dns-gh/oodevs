// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Omt13String.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( omt13string_serializes_null_terminated_string, SerializationFixture )
{
    const std::string message = "message";
    const Omt13String omt( message );
    ::hla::Deserializer deserializer = Serialize( omt,
        static_cast< unsigned int>( sizeof( int8_t ) * ( message.size() + 1 )) );
    BOOST_CHECK_EQUAL( 'm', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL( 'e', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL( 's', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL( 's', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL( 'a', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL( 'g', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL( 'e', Read< int8_t >( deserializer ) );
    BOOST_CHECK_EQUAL(  0 , Read< int8_t >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( omt13string_deserializes_null_terminated_string, SerializationFixture )
{
    const std::string message = "message";
    const Omt13String serializedOmt( message );
    ::hla::Deserializer deserializer = Serialize( serializedOmt,
        static_cast< unsigned int >( sizeof( int8_t ) * ( message.size() + 1 )) );
    Omt13String deserializedOmt;
    deserializedOmt.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( message, deserializedOmt.str() );
}

BOOST_FIXTURE_TEST_CASE( omt13string_deserializes_twice, SerializationFixture )
{
    const std::string message = "message";
    Omt13String deserializedOmt;
    {
        const Omt13String serializedOmt( message );
        ::hla::Deserializer deserializer = Serialize( serializedOmt,
            static_cast< unsigned int >( sizeof( int8_t ) * ( message.size() + 1 )) );
        deserializedOmt.Deserialize( deserializer );
        BOOST_CHECK_EQUAL( message, deserializedOmt.str() );
    }
    {
        const Omt13String serializedOmt( message );
        ::hla::Deserializer deserializer = Serialize( serializedOmt,
            static_cast< unsigned int >( sizeof( int8_t ) * ( message.size() + 1 )) );
        deserializedOmt.Deserialize( deserializer );
        BOOST_CHECK_EQUAL( message, deserializedOmt.str() );
    }
}
