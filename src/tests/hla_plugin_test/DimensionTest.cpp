// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Dimension.h"
#include "Tools.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( dimension_on_road_serialization, SerializationFixture )
{
    const Dimension dimensions( true );
    ::hla::Deserializer deserializer = Serialize( dimensions, 3 * sizeof( real32 ) );
    const float xAxisOnRoad = 200.;
    const float yAxisOnRoad = 50.;
    const float zAxisOnRoad = 3.;
    BOOST_CHECK_EQUAL( xAxisOnRoad, Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( yAxisOnRoad, Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( zAxisOnRoad, Read< real32 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( dimension_on_ground_serialization, SerializationFixture )
{
    const Dimension dimensions( false );
    ::hla::Deserializer deserializer = Serialize( dimensions, 3 * sizeof( real32 ) );
    const float xAxisOnRoad = 100.;
    const float yAxisOnRoad = 100.;
    const float zAxisOnRoad = 3.;
    BOOST_CHECK_EQUAL( xAxisOnRoad, Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( yAxisOnRoad, Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( zAxisOnRoad, Read< real32 >( deserializer ) );
}
