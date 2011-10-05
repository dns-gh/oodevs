// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/AggregateMarking.h"
#include "hla_plugin/SerializationTools.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( aggregate_marking_serializes_in_ascii_on_first_byte, SerializationFixture )
{
    const AggregateMarking marking( "name" );
    ::hla::Deserializer deserializer = Serialize( marking, 32 * sizeof( int8 ) );
    const int8 asciiEncoding = 1;
    BOOST_CHECK_EQUAL( asciiEncoding, Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( aggregate_marking_serializes_name_in_a_31_characters_buffer, SerializationFixture )
{
    const AggregateMarking marking( "name" );
    ::hla::Deserializer deserializer = Serialize( marking, 32 * sizeof( int8 ) );
    Read< int8 >( deserializer );
    BOOST_CHECK_EQUAL( 'n', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'a', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'm', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL( 'e', Read< int8 >( deserializer ) );
    BOOST_CHECK_EQUAL(  0 , Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( aggregate_marking_truncates_name_over_31_characters, SerializationFixture )
{
    const AggregateMarking marking( "big_name_too_long_for_a_small_buffer" );
    ::hla::Deserializer deserializer = Serialize( marking, 32 * sizeof( int8 ) );
    for( unsigned int i = 0; i < 31; ++i )
        Read< int8 >( deserializer );
    BOOST_CHECK_EQUAL( 'b', Read< int8 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( aggregate_marking_deserializes_to_string, SerializationFixture )
{
    const AggregateMarking serializedMarking( "name" );
    ::hla::Deserializer deserializer = Serialize( serializedMarking, 32 * sizeof( int8 ) );
    AggregateMarking deserializedMarking;
    deserializedMarking.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( "name", deserializedMarking.str() );
}

BOOST_FIXTURE_TEST_CASE( marking_deserializes_truncated_string, SerializationFixture )
{
    const Marking serializedMarking( "big_name_too_long_for_a_small_buffer" );
    ::hla::Deserializer deserializer = Serialize( serializedMarking, 12 * sizeof( int8 ) );
    Marking deserializedMarking;
    deserializedMarking.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( "big_name_to", deserializedMarking.str() );
}

BOOST_FIXTURE_TEST_CASE( marking_truncates_name_over_11_characters, SerializationFixture )
{
    const Marking marking( "big_name_too_long_for_a_small_buffer" );
    ::hla::Deserializer deserializer = Serialize( marking, 12 * sizeof( int8 ) );
    for( unsigned int i = 0; i < 11; ++i )
        Read< int8 >( deserializer );
    BOOST_CHECK_EQUAL( 'o', Read< int8 >( deserializer ) );
}
