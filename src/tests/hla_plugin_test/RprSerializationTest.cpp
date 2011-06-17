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
#include "rpr/Coordinates.h"
#include "Tools.h"

using namespace rpr;

BOOST_FIXTURE_TEST_CASE( entity_identifier_serialization, SerializationFixture )
{
    const unsigned short site = 1;
    const unsigned short application = 2;
    const unsigned short number = 3;
    const EntityIdentifier identifier( site, application, number );
    ::hla::Deserializer deserializer = Serialize( identifier, 3 * sizeof( int16 ) );
    BOOST_CHECK_EQUAL( site       , Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( application, Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( number     , Read< unsigned short >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( entity_type_serialization, SerializationFixture )
{
    const unsigned char entityKind = 1;
    const unsigned char domain = 2;
    const unsigned short countryCode = 3;
    const unsigned char category = 4;
    const unsigned char subcategory = 5;
    const unsigned char specific = 6;
    const unsigned char extra = 7;
    const EntityType type( "1 2 3 4 5 6 7" );
    ::hla::Deserializer deserializer = Serialize( type, 6 * sizeof( int8 ) + 1 * sizeof( int16 ) );
    BOOST_CHECK_EQUAL( entityKind,  Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( domain,      Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( countryCode, Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( category,    Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( subcategory, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( specific,    Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( extra,       Read< unsigned char >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( entity_type_can_be_truncated, SerializationFixture )
{
    const unsigned char entityKind = 1;
    const unsigned char domain = 2;
    const EntityType type( "1 2" );
    ::hla::Deserializer deserializer = Serialize( type, 6 * sizeof( int8 ) + 1 * sizeof( int16 ) );
    BOOST_CHECK_EQUAL( entityKind, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( domain,     Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned short >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
    BOOST_CHECK_EQUAL( 0, Read< unsigned char >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( world_location_serialization, SerializationFixture )
{
    const WorldLocation location( 1., 2., 3. );
    ::hla::Deserializer deserializer = Serialize( location, 3 * sizeof( real64 ) );
    BOOST_CHECK_EQUAL( location.X(), Read< real64 >( deserializer ) );
    BOOST_CHECK_EQUAL( location.Y(), Read< real64 >( deserializer ) );
    BOOST_CHECK_EQUAL( location.Z(), Read< real64 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( velocity_vector_serialization, SerializationFixture )
{
    const WorldLocation location( 1., 2., 3. );
    const VelocityVector velocity( location, 4., 5. );
    ::hla::Deserializer deserializer = Serialize( velocity, 3 * sizeof( real32 ) );
    BOOST_CHECK_EQUAL( velocity.VX(), Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( velocity.VY(), Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( velocity.VZ(), Read< real32 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( orientation_serialization, SerializationFixture )
{
    const WorldLocation location( 1., 2., 3. );
    const VelocityVector velocity( location, 4., 5. );
    const Orientation orientation( location, velocity );
    ::hla::Deserializer deserializer = Serialize( orientation, 3 * sizeof( real32 ) );
    BOOST_CHECK_EQUAL( orientation.Psi(), Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( orientation.Theta(), Read< real32 >( deserializer ) );
    BOOST_CHECK_EQUAL( orientation.Phi(), Read< real32 >( deserializer ) );
}
