// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/UniqueId.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( unique_id_serializes_identifier_in_a_11_characters_buffer, SerializationFixture )
{
    const UniqueId id( "SWORD1" );
    ::hla::Deserializer deserializer = Serialize( id, 11 * sizeof( int8 ) );
    BOOST_CHECK_EQUAL( 'S', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'W', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'O', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'R', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'D', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( '1', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL(  0 , Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( unique_id_truncates_name_over_11_characters, SerializationFixture )
{
    const UniqueId id( "big_name_too_long_for_a_small_buffer" );
    ::hla::Deserializer deserializer = Serialize( id, 11 * sizeof( int8 ) );
    for( unsigned int i = 0; i < 10; ++i )
        Read< int8 >( deserializer );
    BOOST_CHECK_EQUAL( 'o', Read< int8 >( deserializer ) );
}
