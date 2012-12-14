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
        rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */+ 
        4*sizeof( unsigned short ) +2 /* padding */ );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_deserializes, SerializationFixture )
{
    Omt13String rtiId( "anRtiId" );
    const IsPartOfStruct isPartOf( rpr::EntityIdentifier( 12, 13, 14 ), rtiId );
    ::hla::Deserializer deserializer = Serialize( isPartOf,
            3*sizeof( unsigned short) + 
            rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */+ 
            4*sizeof( unsigned short ) +2 /* padding */  );
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
        rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */+ 
        4*sizeof( unsigned short ) +4 /* padding */+
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
            3*sizeof( unsigned short) + 
            rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */+ 
            4*sizeof( unsigned short ) +4 /* padding */+ 3*sizeof( float ) );
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
        rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */+ // RTI ID
        4*sizeof( unsigned short )  +4 /* padding */+
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
            3*sizeof( unsigned short) + 
            rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */+ 
            4*sizeof( unsigned short ) +4 /* padding */+ 2*sizeof( float ) );
    IsPartOfStruct deserializedStruct;
    deserializedStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( isPartOf.rtiId_.str(), deserializedStruct.rtiId_.str() );
    BOOST_REQUIRE( isPartOf.disId_ == deserializedStruct.disId_ );
    BOOST_CHECK_EQUAL( deserializedStruct.constituentPartStationName, IsPartOfStruct::OnStationRangeBearing );
    BOOST_CHECK_EQUAL( deserializedStruct.range, isPartOf.range );
    BOOST_CHECK_EQUAL( deserializedStruct.bearing, isPartOf.bearing );
}

BOOST_FIXTURE_TEST_CASE( is_part_of_serialize_example, SerializationFixture )
{
    // From Pitch
    // IsPartOf = {205.1.270, HLAobjectRoot.BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel370, {ComputerGeneratedForcesCarriedAsCargo, Inside}, {0, (OnStationXYZ: RelativeLocation = {0.0, 0.0, 0.0})}}
    // 00CD0001 010E484C 416F626A 65637452 6F6F742E 42617365 456E7469 74792E50 68797369 63616C45 6E746974 792E506C 6174666F 726D2E53 75726661 63655665 7373656C 2E4E4554 4E5F5375 72666163 65566573 73656C33 37300000 00050002 00000000 00100000 00000000 00000000 00000000

    Omt13String rtiId( "HLAobjectRoot.BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel370" );
    IsPartOfStruct isPartOf( rpr::EntityIdentifier( 205, 1, 270 ), rtiId );
    isPartOf.constituentPartNature_ = static_cast< unsigned short >( IsPartOfStruct::constituentPartNatureEnum16_ComputerGeneratedForcesCarriedAsCargo );
    isPartOf.constituentPartPosition_ = static_cast< unsigned short>( IsPartOfStruct::constituentPartPosition_Inside );
    isPartOf.stationNumber_ = 0;
    isPartOf.constituentPartStationName = IsPartOfStruct::OnStationXYZ;
    isPartOf.x = isPartOf.y = isPartOf.z = 0;
    ::hla::Deserializer deserializer = Serialize( isPartOf,
            3*sizeof( unsigned short) + 
            rtiId.str().size()+1 + (rtiId.str().size() % 2 == 0 ? 1 : 0) /* padding */ +
            4*sizeof( unsigned short ) + 4 /* padding */ +
            3*sizeof( float ) );
    char expected[] = { "\x00\xCD\x00\x01" "\x01\x0E\x48\x4C" "\x41\x6F\x62\x6A" "\x65\x63\x74\x52" "\x6F\x6F\x74\x2E" "\x42\x61\x73\x65" "\x45\x6E\x74\x69" 
                        "\x74\x79\x2E\x50" "\x68\x79\x73\x69" "\x63\x61\x6C\x45" "\x6E\x74\x69\x74" "\x79\x2E\x50\x6C" "\x61\x74\x66\x6F" "\x72\x6D\x2E\x53"
                        "\x75\x72\x66\x61" "\x63\x65\x56\x65" "\x73\x73\x65\x6C" "\x2E\x4E\x45\x54" "\x4E\x5F\x53\x75" "\x72\x66\x61\x63" "\x65\x56\x65\x73"
                        "\x73\x65\x6C\x33" "\x37\x30\x00\x00" "\x00\x05\x00\x02" "\x00\x00\x00\x00" "\x00\x10\x00\x00" "\x00\x00\x00\x00" "\x00\x00\x00\x00"
                        "\x00\x00\x00\x00" };
    std::vector< char > expectV( expected, expected+sizeof(expected)-1 );
    std::vector< char > received( deserializer.GetSize() );
    BOOST_CHECK_EQUAL( deserializer.GetSize(), expectV.size() );
    deserializer.CopyTo( &received[0] );
    BOOST_CHECK( received ==  expectV );

    IsPartOfStruct deserializedStruct;
    deserializedStruct.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( isPartOf.rtiId_.str(), deserializedStruct.rtiId_.str() );
    BOOST_REQUIRE( isPartOf.disId_ == deserializedStruct.disId_ );
    BOOST_CHECK_EQUAL( deserializedStruct.constituentPartStationName, IsPartOfStruct::OnStationXYZ );
    BOOST_CHECK_EQUAL( deserializedStruct.x, isPartOf.x );
    BOOST_CHECK_EQUAL( deserializedStruct.y, isPartOf.y );
    BOOST_CHECK_EQUAL( deserializedStruct.z, isPartOf.z );
}
