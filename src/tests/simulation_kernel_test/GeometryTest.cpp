// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Tools/MIL_Geometry.h"
#include "simulation_terrain/TER_Polygon.h"

// -----------------------------------------------------------------------------
// Name: TestComputePolygonHull

// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestComputePolygonHull )
{
    MT_Vector2D Point1( 0, 0 );
    MT_Vector2D Point2( 0, 10 );
    MT_Vector2D Point3( 10, 10 );
    MT_Vector2D Point4( 10, 0 );
    MT_Vector2D Point5( 10, -10 );
    MT_Vector2D Point6( 0, -10 );
    MT_Vector2D Point7( -10, -10 );
    MT_Vector2D Point8( -10, 0 );
    T_PointVector input;
    input.push_back( Point1 );
    input.push_back( Point2 );
    input.push_back( Point3 );
    input.push_back( Point4 );
    input.push_back( Point5 );
    input.push_back( Point6 );
    input.push_back( Point7 );
    input.push_back( Point8 );
    T_PointVector output;
    MIL_Geometry::ComputeHull( output, input );

    T_PointVector result;
    result.push_back( Point2 );
    result.push_back( Point8 );
    result.push_back( Point7 );
    result.push_back( Point5 );
    result.push_back( Point3 );
    BOOST_CHECK_EQUAL( true, output == result );
}

// -----------------------------------------------------------------------------
// Name: TestComputePolygonScale
// Created: ???
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestComputePolygonScale )
{
    MT_Vector2D Point1( -10, 0 );
    MT_Vector2D Point2( 0, 10 );
    MT_Vector2D Point3( 10, 0 );
    MT_Vector2D Point4( 0, 0 );
    MT_Vector2D Point5( 0, -10 );
    T_PointVector input;
    input.push_back( Point1 );
    input.push_back( Point2 );
    input.push_back( Point3 );
    input.push_back( Point4 );
    input.push_back( Point5 );
    TER_Polygon output;
    MIL_Geometry::Scale( output, input, 10.f );
    T_PointVector result;
    MT_Vector2D Point6( -20, 0 );
    MT_Vector2D Point7( 0, 20 );
    MT_Vector2D Point8( 20, 0 );
    MT_Vector2D Point9( 0, -20 );
    result.push_back( Point9 );
    result.push_back( Point8 );
    result.push_back( Point7 );
    result.push_back( Point6 );
    BOOST_CHECK_EQUAL( true, output.GetBorderPoints() == result );
}
