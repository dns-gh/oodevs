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
    ::hla::Deserializer deserializer = Serialize( unicode, sizeof( uint32 ) + message.size() * sizeof( uint16 ) );
    BOOST_CHECK_EQUAL( message.size(), Read< uint32 >( deserializer ) );
    BOOST_CHECK_EQUAL( wchar_t( 'm' ), Read< uint16 >( deserializer ) );
}
