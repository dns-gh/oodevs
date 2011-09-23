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
    ::hla::Deserializer deserializer = Serialize( omt, sizeof( int8 ) * ( message.size() + 1 ) );
    BOOST_CHECK_EQUAL( 'm', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'e', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 's', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 's', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'a', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'g', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'e', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL(  0 , Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( omt13string_deserializes_null_terminated_string, SerializationFixture )
{
    const std::string message = "message";
    const Omt13String serializedOmt( message );
    ::hla::Deserializer deserializer = Serialize( serializedOmt, sizeof( int8 ) * ( message.size() + 1 ) );
    Omt13String deserializedOmt;
    deserializedOmt.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( message, deserializedOmt.str() );
}
