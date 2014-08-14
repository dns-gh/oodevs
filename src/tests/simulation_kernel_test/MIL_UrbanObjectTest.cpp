// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Urban/MIL_UrbanObject.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"
#include "MockMIL_ObjectType_ABC.h"

BOOST_AUTO_TEST_CASE( scale_urban_location )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    MockMIL_ObjectType_ABC type;
    MOCK_EXPECT( type.GetPointSize ).returns( 1 );
    MIL_UrbanObject object( 0, "urban", type );
    T_PointVector points( 4 );
    points[ 0 ] = MT_Vector2D( 0, 0 );
    points[ 1 ] = MT_Vector2D( 0, 1 );
    points[ 2 ] = MT_Vector2D( 1, 1 );
    points[ 3 ] = MT_Vector2D( 1, 0 );
    const TER_Localisation localisation( TER_Localisation::ePolygon, points );
    object.Initialize( localisation );
    TER_Polygon p1 = object.GetScaledLocation( 1 );
    p1 = object.GetScaledLocation( 1 );
    BOOST_CHECK_EQUAL( "POLYGON (0 -1, 1 -1, 1.70711 -0.707107, 2 0, 2 1, 1.70711 1.70711, 1 2, 0 2, -0.707107 1.70711, -1 1, -1 0, -0.707107 -0.707107, 0 -1)", ToWKT( p1 ) );
    // What we'd really want is to measure performance of the method. Just checking getting past the max number is ok.
    for( double i = 0; i < MIL_UrbanObject::maxScaledLocationsNumber_ + 2; ++i )
        object.GetScaledLocation( i );
    TER_Polygon p0 = object.GetScaledLocation( 0 );
    p1 = object.GetScaledLocation( 1 );
    TER_Polygon p2 = object.GetScaledLocation( 2 );
    TER_Polygon p3 = object.GetScaledLocation( 3 );
    p3 = object.GetScaledLocation( 3 );
    BOOST_CHECK_EQUAL( "POLYGON (0 0, 1 0, 1 1, 0 1, 0 0)", ToWKT( p0 ) );
    BOOST_CHECK_EQUAL( "POLYGON (0 -1, 1 -1, 1.70711 -0.707107, 2 0, 2 1, 1.70711 1.70711, 1 2, 0 2, -0.707107 1.70711, -1 1, -1 0, -0.707107 -0.707107, 0 -1)", ToWKT( p1 ) );
    BOOST_CHECK_EQUAL( "POLYGON (0 -2, 1 -2, 2.41421 -1.41421, 3 0, 3 1, 2.41421 2.41421, 1 3, 0 3, -1.41421 2.41421, -2 1, -2 0, -1.41421 -1.41421, 0 -2)", ToWKT( p2 ) );
    BOOST_CHECK_EQUAL( "POLYGON (0 -3, 1 -3, 3.12132 -2.12132, 4 0, 4 1, 3.12132 3.12132, 1 4, 0 4, -2.12132 3.12132, -3 1, -3 0, -2.12132 -2.12132, 0 -3)", ToWKT( p3 ) );
}