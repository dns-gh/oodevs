// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "rpr/Coordinates.h"
#include "SerializationFixture.h"

using namespace rpr;

BOOST_FIXTURE_TEST_CASE( world_location_deserialization, SerializationFixture )
{
    const WorldLocation serializedLocation( 1., 2., 3. );
    ::hla::Deserializer deserializer = Serialize( serializedLocation, 3 * sizeof( real64 ) );
    WorldLocation deserializedLocation;
    deserializedLocation.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( serializedLocation.X(), deserializedLocation.X() );
    BOOST_CHECK_EQUAL( serializedLocation.Y(), deserializedLocation.Y() );
    BOOST_CHECK_EQUAL( serializedLocation.Z(), deserializedLocation.Z() );
}

BOOST_FIXTURE_TEST_CASE( velocity_vector_deserialization, SerializationFixture )
{
    const WorldLocation location( 1., 2., 3. );
    const VelocityVector serializedVelocity( location, 4., 5. );
    ::hla::Deserializer deserializer = Serialize( serializedVelocity, 3 * sizeof( real32 ) );
    VelocityVector deserializedVelocity;
    deserializedVelocity.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( serializedVelocity.VX(), deserializedVelocity.VX() );
    BOOST_CHECK_EQUAL( serializedVelocity.VY(), deserializedVelocity.VY() );
    BOOST_CHECK_EQUAL( serializedVelocity.VZ(), deserializedVelocity.VZ() );
}

BOOST_FIXTURE_TEST_CASE( orientation_deserialization, SerializationFixture )
{
    const WorldLocation location( 1., 2., 3. );
    const VelocityVector velocity( location, 4., 5. );
    const Orientation serializedOrientation( location, velocity );
    ::hla::Deserializer deserializer = Serialize( serializedOrientation, 3 * sizeof( real32 ) );
    Orientation deserializedOrientation;
    deserializedOrientation.Deserialize( deserializer );
    BOOST_CHECK_EQUAL( serializedOrientation.Psi()  , deserializedOrientation.Psi() );
    BOOST_CHECK_EQUAL( serializedOrientation.Theta(), deserializedOrientation.Theta() );
    BOOST_CHECK_EQUAL( serializedOrientation.Phi()  , deserializedOrientation.Phi() );
}
