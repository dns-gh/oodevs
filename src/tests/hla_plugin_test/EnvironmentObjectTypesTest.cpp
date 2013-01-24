// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "rpr/BreachableSegmentStruct.h"
#include "SerializationFixture.h"
#include <algorithm>

using namespace plugins::hla;

namespace
{
    const std::size_t segmentSize = 2 * sizeof( uint32_t ) + // SegmentNumber, PercentComplete
                                    3 * sizeof( real64 ) + // WorldLocation
                                    3 * sizeof( real32 ) + // Orientation
                                    4 * sizeof( uint16_t ) + // Length, width, Height, Depth
                                    sizeof( uint32_t ) + // DamagedState
                                    4 * sizeof( uint8_t );
    const std::size_t breachableSize = segmentSize +
                                       sizeof( uint32_t ) + // Breachlength
                                       sizeof( uint8_t ) + // BreachedState
                                       8 * sizeof( uint8_t ); // SegmentBreached
}

BOOST_FIXTURE_TEST_CASE( linearsegmentstruct_deserialization_size, SerializationFixture )
{
    rpr::LinearSegmentStruct serializedSegment;
    ::hla::Deserializer deserializer = Serialize( serializedSegment, segmentSize );
    rpr::LinearSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
}

namespace rpr
{
    std::ostream& operator<<( std::ostream&, const rpr::WorldLocation& );
    bool operator== (const WorldLocation& lhs, const WorldLocation& rhs );
    bool operator== (const Orientation& lhs, const Orientation& rhs );
    std::ostream& operator<<( std::ostream& os, const rpr::Orientation& loc)
    {
        return os << "(" << loc.Psi() << "," << loc.Theta() << ","<< loc.Phi() << ")";
    }
}

BOOST_FIXTURE_TEST_CASE( linearsegmentstruct_deserialization_location, SerializationFixture )
{
    rpr::LinearSegmentStruct serializedSegment;
    serializedSegment.location_ = rpr::WorldLocation( 12.0, 13.0, 14.0 );
    ::hla::Deserializer deserializer = Serialize( serializedSegment, segmentSize );
    rpr::LinearSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedSegment.location_, serializedSegment.location_ );
}

BOOST_FIXTURE_TEST_CASE( linearsegmentstruct_deserialization_orientation, SerializationFixture )
{
    rpr::LinearSegmentStruct serializedSegment;
    serializedSegment.orientation_ = rpr::Orientation( 12.0, 13.0, 14.0 );
    ::hla::Deserializer deserializer = Serialize( serializedSegment, segmentSize );
    rpr::LinearSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedSegment.orientation_, serializedSegment.orientation_ );
}

BOOST_FIXTURE_TEST_CASE( linearsegmentstruct_deserialization_other, SerializationFixture )
{
    rpr::LinearSegmentStruct serializedSegment;
    serializedSegment.segmentNumber_=42;
    serializedSegment.percentComplete_=43;
    serializedSegment.length_=44;
    serializedSegment.width_=45;
    serializedSegment.height_=46;
    serializedSegment.depth_=47;
    serializedSegment.damagedState_=rpr::damageState_ModerateDamage;
    serializedSegment.deactivated_ = true;
    serializedSegment.flaming_ = true;
    serializedSegment.predistributed_ = true;
    serializedSegment.smoking_ = true;
    ::hla::Deserializer deserializer = Serialize( serializedSegment, segmentSize );
    rpr::LinearSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedSegment.segmentNumber_, serializedSegment.segmentNumber_ );
    BOOST_CHECK_EQUAL( deserializedSegment.percentComplete_, serializedSegment.percentComplete_ );
    BOOST_CHECK_EQUAL( deserializedSegment.length_, serializedSegment.length_ );
    BOOST_CHECK_EQUAL( deserializedSegment.width_, serializedSegment.width_ );
    BOOST_CHECK_EQUAL( deserializedSegment.height_, serializedSegment.height_ );
    BOOST_CHECK_EQUAL( deserializedSegment.depth_, serializedSegment.depth_ );
    BOOST_CHECK_EQUAL( deserializedSegment.damagedState_, serializedSegment.damagedState_ );
    BOOST_CHECK_EQUAL( deserializedSegment.deactivated_, serializedSegment.deactivated_ );
    BOOST_CHECK_EQUAL( deserializedSegment.flaming_, serializedSegment.flaming_ );
    BOOST_CHECK_EQUAL( deserializedSegment.predistributed_, serializedSegment.predistributed_ );
    BOOST_CHECK_EQUAL( deserializedSegment.flaming_, serializedSegment.flaming_ );
    BOOST_CHECK_EQUAL( deserializedSegment.smoking_, serializedSegment.smoking_ );
}

BOOST_FIXTURE_TEST_CASE( breachablesegmentstruct_deserialization_size, SerializationFixture )
{
    rpr::BreachableSegmentStruct serializedSegment;
    ::hla::Deserializer deserializer = Serialize( serializedSegment, breachableSize );
    rpr::BreachableSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
}

BOOST_FIXTURE_TEST_CASE( breachablesegmentstruct_deserialization_location, SerializationFixture )
{
    rpr::BreachableSegmentStruct serializedSegment;
    serializedSegment.segmentParameters_.location_ = rpr::WorldLocation( 12.0, 13.0, 14.0 );
    ::hla::Deserializer deserializer = Serialize( serializedSegment, breachableSize );
    rpr::BreachableSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedSegment.segmentParameters_.location_, serializedSegment.segmentParameters_.location_ );
}

BOOST_FIXTURE_TEST_CASE( breachablesegmentstruct_deserialization_orientation, SerializationFixture )
{
    rpr::BreachableSegmentStruct serializedSegment;
    serializedSegment.segmentParameters_.orientation_ = rpr::Orientation( 12.0, 13.0, 14.0 );
    ::hla::Deserializer deserializer = Serialize( serializedSegment, breachableSize );
    rpr::BreachableSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedSegment.segmentParameters_.orientation_, serializedSegment.segmentParameters_.orientation_ );
}

BOOST_FIXTURE_TEST_CASE( breachablesegmentstruct_deserialization_other, SerializationFixture )
{
    rpr::BreachableSegmentStruct serializedSegment;
    serializedSegment.breachedLength_ = 42;
    serializedSegment.breachedState_ = rpr::breachedStatus_ModerateBreaching;
    rpr::BreachedStatusEnum8 tmp[] = { rpr::breachedStatus_SlightBreaching, rpr::breachedStatus_Cleared,
                                            rpr::breachedStatus_Cleared, rpr::breachedStatus_SlightBreaching,
                                            rpr::breachedStatus_SlightBreaching, rpr::breachedStatus_NoBreaching,
                                            rpr::breachedStatus_ModerateBreaching, rpr::breachedStatus_NoBreaching };
    std::copy( std::begin( tmp ), std::end( tmp ), std::begin( serializedSegment.segmentBreached_ ) );
    ::hla::Deserializer deserializer = Serialize( serializedSegment, breachableSize );
    rpr::BreachableSegmentStruct deserializedSegment;
    deserializedSegment.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( deserializedSegment.breachedLength_, serializedSegment.breachedLength_ );
    BOOST_CHECK_EQUAL( deserializedSegment.breachedState_, serializedSegment.breachedState_ );
    BOOST_CHECK( std::equal( std::begin( deserializedSegment.segmentBreached_ ), std::end( deserializedSegment.segmentBreached_ ), std::begin( tmp ) ) );
}
