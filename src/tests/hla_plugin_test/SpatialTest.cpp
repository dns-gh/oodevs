// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Spatial.h"
#include "hla_plugin/SerializationTools.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( static_dead_reckoning_algorithm, SerializationFixture )
{
    const Spatial spatial( true, 1., 2., 3., 4., 5. );
    const unsigned int paddingSize = sizeof( int8 );
    const unsigned int spatialStructHeaderSize = sizeof( int8 ) + 7 * paddingSize;
    const unsigned int worldLocationSize = 3 * sizeof( real64 );
    const unsigned int isFrozenSize = sizeof( int8 ) + 3 * paddingSize;
    const unsigned int orientationSize = 3 * sizeof( real32 );
    const unsigned int spatialSize = spatialStructHeaderSize
                                   + worldLocationSize
                                   + isFrozenSize
                                   + orientationSize;
    ::hla::Deserializer deserializer = Serialize( spatial, spatialSize );

    const int8 staticAlgorithm = 1;
    BOOST_CHECK_EQUAL( staticAlgorithm, Read< int8 >( deserializer ) );
    ReadPadding< 7 >( deserializer );
    Read< worldLocationSize >( deserializer );
    const int8 notFrozen = 0;
    BOOST_CHECK_EQUAL( notFrozen, Read< int8 >( deserializer ) );
    ReadPadding< 3 >( deserializer );
    Read< orientationSize >( deserializer );
}

BOOST_FIXTURE_TEST_CASE( fixed_rotation_and_rate_of_position_dead_reckoning_algorithm, SerializationFixture )
{
    const Spatial spatial( false, 1., 2., 3., 4., 5. );
    const unsigned int paddingSize = sizeof( int8 );
    const unsigned int spatialStructHeaderSize = sizeof( int8 ) + 7 * paddingSize;
    const unsigned int worldLocationSize = 3 * sizeof( real64 );
    const unsigned int isFrozenSize = sizeof( int8 ) + 3 * paddingSize;
    const unsigned int orientationSize = 3 * sizeof( real32 );
    const unsigned int velocitySize = 3 * sizeof( real32 );
    const unsigned int spatialSize = spatialStructHeaderSize
                                   + worldLocationSize
                                   + isFrozenSize
                                   + orientationSize
                                   + velocitySize;
    ::hla::Deserializer deserializer = Serialize( spatial, spatialSize );

    const int8 DRM_FPW_Algorithm = 2;
    BOOST_CHECK_EQUAL( DRM_FPW_Algorithm, Read< int8 >( deserializer ) );
    ReadPadding< 7 >( deserializer );
    Read< worldLocationSize >( deserializer );
    const int8 notFrozen = 0;
    BOOST_CHECK_EQUAL( notFrozen, Read< int8 >( deserializer ) );
    ReadPadding< 3 >( deserializer );
    Read< orientationSize >( deserializer );
    Read< velocitySize >( deserializer );
}
