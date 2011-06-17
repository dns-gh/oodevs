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
#include <hla/Serializer.h>
#include <hla/Deserializer.h>

using namespace plugins::hla;

namespace
{
    typedef std::vector< int8 > T_Buffer;
    T_Buffer Convert( const ::hla::Serializer& serializer )
    {
        T_Buffer buffer( serializer.GetSize() );
        if( !buffer.empty() )
            serializer.CopyTo( &buffer[0] );
        return buffer;
    }
}

BOOST_AUTO_TEST_CASE( aggregate_marking_serializes_in_ascii_on_first_byte )
{
    const AggregateMarking marking( "name" );
    ::hla::Serializer serializer;
    marking.Serialize( serializer );
    const T_Buffer buffer = Convert( serializer );
    const int8 asciiEncoding = 1;
    BOOST_CHECK_EQUAL( asciiEncoding, buffer.at( 0 ) );
}

BOOST_AUTO_TEST_CASE( aggregate_marking_serializes_name_in_a_31_characters_buffer )
{
    const AggregateMarking marking( "name" );
    ::hla::Serializer serializer;
    marking.Serialize( serializer );
    const T_Buffer buffer = Convert( serializer );
    BOOST_CHECK_EQUAL( 32u, buffer.size() );
    BOOST_CHECK_EQUAL( 'n', buffer.at( 1 ) );
    BOOST_CHECK_EQUAL( 'a', buffer.at( 2 ) );
    BOOST_CHECK_EQUAL( 'm', buffer.at( 3 ) );
    BOOST_CHECK_EQUAL( 'e', buffer.at( 4 ) );
    BOOST_CHECK_EQUAL(  0 , buffer.at( 5 ) );
}

BOOST_AUTO_TEST_CASE( aggregate_marking_truncates_name_over_31_characters )
{
    const AggregateMarking marking( "big_name_too_long_for_a_small_buffer" );
    ::hla::Serializer serializer;
    marking.Serialize( serializer );
    const T_Buffer buffer = Convert( serializer );
    BOOST_CHECK_EQUAL( 32u, buffer.size() );
    BOOST_CHECK_EQUAL( 'b', buffer.at( 31 ) );
}
