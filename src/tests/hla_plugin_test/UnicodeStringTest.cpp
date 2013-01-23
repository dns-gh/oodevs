// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/UnicodeString.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( unicode_string_serializes_size_and_content, SerializationFixture )
{
    const std::string message = "message";
    const UnicodeString unicode( message );
    ::hla::Deserializer deserializer = Serialize( unicode,
        static_cast< unsigned int >( sizeof( uint32_t ) + message.size() * sizeof( uint16_t ) ));
    BOOST_CHECK_EQUAL( message.size(), Read< uint32_t >( deserializer ) );
    BOOST_CHECK_EQUAL( wchar_t( 'm' ), Read< uint16_t >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( unicode_string_deserializes, SerializationFixture )
{
    const std::string message = "message";
    const UnicodeString serializedUnicode( message );
    ::hla::Deserializer deserializer = Serialize( serializedUnicode,
        static_cast< unsigned int >( sizeof( uint32_t ) + message.size() * sizeof( uint16_t ) ));
    UnicodeString deserializedUnicode;
    deserializedUnicode.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( message, deserializedUnicode.str() );
}
