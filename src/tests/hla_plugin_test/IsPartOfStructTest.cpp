// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/IsPartOfStruct.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( is_part_of_serializes, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    const IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    ::hla::Deserializer deserializer = Serialize( isPartOf,
        3*sizeof( unsigned short) + // DIS ID
        rtiId.str().size()+1 + // RTI ID
        4*sizeof( unsigned short ) );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_deserializes, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    const IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    ::hla::Deserializer deserializer = Serialize( isPartOf,
            3*sizeof( unsigned short) + rtiId.str().size()+1 + 4*sizeof( unsigned short )  );
    IsPartOfStruct deserializedStruct;
    deserializedStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( isPartOf.rtiId_.str(), deserializedStruct.rtiId_.str() );
    BOOST_REQUIRE( isPartOf.disId_ == deserializedStruct.disId_ );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_serializes_with_xyz, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    isPartOf.constituentPartStationName = IsPartOfStruct::OnStationXYZ;
    isPartOf.x = 22.0f;
    isPartOf.y = 23.0f;
    isPartOf.z = 24.0f;
    ::hla::Deserializer deserializer = Serialize( isPartOf,
        3*sizeof( unsigned short) + // DIS ID
        rtiId.str().size()+1 + // RTI ID
        4*sizeof( unsigned short ) +
        3*sizeof( float ) );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_deserializes_with_xyz, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    isPartOf.constituentPartStationName = IsPartOfStruct::OnStationXYZ;
    isPartOf.x = 22.0f;
    isPartOf.y = 23.0f;
    isPartOf.z = 24.0f;
    ::hla::Deserializer deserializer = Serialize( isPartOf,
            3*sizeof( unsigned short) + rtiId.str().size()+1 + 4*sizeof( unsigned short ) + 3*sizeof( float ) );
    IsPartOfStruct deserializedStruct;
    deserializedStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( isPartOf.rtiId_.str(), deserializedStruct.rtiId_.str() );
    BOOST_REQUIRE( isPartOf.disId_ == deserializedStruct.disId_ );
    BOOST_CHECK_EQUAL( deserializedStruct.constituentPartStationName, IsPartOfStruct::OnStationXYZ );
    BOOST_CHECK_EQUAL( deserializedStruct.x, isPartOf.x );
    BOOST_CHECK_EQUAL( deserializedStruct.y, isPartOf.y );
    BOOST_CHECK_EQUAL( deserializedStruct.z, isPartOf.z );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_serializes_with_range_bearing, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    isPartOf.constituentPartStationName = IsPartOfStruct::OnStationRangeBearing;
    isPartOf.range = 22.0f;
    isPartOf.bearing = 23.0f;
    ::hla::Deserializer deserializer = Serialize( isPartOf,
        3*sizeof( unsigned short) + // DIS ID
        rtiId.str().size()+1 + // RTI ID
        4*sizeof( unsigned short )  +
        2*sizeof( float ) );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_deserializes_with_range_bearing, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    isPartOf.constituentPartStationName = IsPartOfStruct::OnStationRangeBearing;
    isPartOf.range = 22.0f;
    isPartOf.bearing = 23.0f;
    ::hla::Deserializer deserializer = Serialize( isPartOf,
            3*sizeof( unsigned short) + rtiId.str().size()+1 + 4*sizeof( unsigned short ) + 2*sizeof( float ) );
    IsPartOfStruct deserializedStruct;
    deserializedStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( isPartOf.rtiId_.str(), deserializedStruct.rtiId_.str() );
    BOOST_REQUIRE( isPartOf.disId_ == deserializedStruct.disId_ );
    BOOST_CHECK_EQUAL( deserializedStruct.constituentPartStationName, IsPartOfStruct::OnStationRangeBearing );
    BOOST_CHECK_EQUAL( deserializedStruct.range, isPartOf.range );
    BOOST_CHECK_EQUAL( deserializedStruct.bearing, isPartOf.bearing );
}
