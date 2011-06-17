// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "Tools.h"

using namespace rpr;

BOOST_AUTO_TEST_CASE( entity_identifier_serialization )
{
    const unsigned short site = 1;
    const unsigned short application = 2;
    const unsigned short number = 3;
    const EntityIdentifier identifier( site, application, number );
    ::hla::Serializer serializer;
    identifier.Serialize( serializer );
    const T_Buffer buffer = Convert( serializer );
    BOOST_CHECK_EQUAL( 3 * sizeof( int16 ), buffer.size() );
    ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
    BOOST_CHECK_EQUAL( site       , Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( application, Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( number     , Read< unsigned short >( deserializer ) );
}

BOOST_AUTO_TEST_CASE( entity_type_serialization )
{
    const unsigned char entityKind = 1;
    const unsigned char domain = 2;
    const unsigned short countryCode = 3;
    const unsigned char category = 4;
    const unsigned char subcategory = 5;
    const unsigned char specific = 6;
    const unsigned char extra = 7;
    const EntityType type( "1 2 3 4 5 6 7" );
    ::hla::Serializer serializer;
    type.Serialize( serializer );
    const T_Buffer buffer = Convert( serializer );
    BOOST_CHECK_EQUAL( 6 * sizeof( int8 ) + 1 * sizeof( int16 ), buffer.size() );
    ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
    BOOST_CHECK_EQUAL( entityKind,  Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( domain,      Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( countryCode, Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( category,    Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( subcategory, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( specific,    Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( extra,       Read< unsigned char >( deserializer ) );
}

BOOST_AUTO_TEST_CASE( entity_type_can_be_truncated )
{
    const unsigned char entityKind = 1;
    const unsigned char domain = 2;
    const EntityType type( "1 2" );
    ::hla::Serializer serializer;
    type.Serialize( serializer );
    const T_Buffer buffer = Convert( serializer );
    BOOST_CHECK_EQUAL( 6 * sizeof( int8 ) + 1 * sizeof( int16 ), buffer.size() );
    ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
    BOOST_CHECK_EQUAL( entityKind, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( domain,     Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
}
