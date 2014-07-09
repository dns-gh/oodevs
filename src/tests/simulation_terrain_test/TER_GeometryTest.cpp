// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_test_pch.h"
#include "simulation_terrain/TER_Geometry.h"
#include "simulation_terrain/TER_Polygon.h"
#include <boost/assign/list_of.hpp>

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
    TER_Geometry::ComputeHull( output, input );

    T_PointVector result;
    result.push_back( Point7 );
    result.push_back( Point5 );
    result.push_back( Point3 );
    result.push_back( Point2 );
    result.push_back( Point8 );
    result.push_back( Point7 );
    BOOST_CHECK_EQUAL_COLLECTIONS( output.begin(), output.end(), result.begin(), result.end() );
}

BOOST_AUTO_TEST_CASE( TestComputePolygonScale )
{
    T_PointVector input = boost::assign::list_of( MT_Vector2D( -10, 0 ) )
                                                ( MT_Vector2D( 0, 10 ) )
                                                ( MT_Vector2D( 10, 0 ) )
                                                ( MT_Vector2D( 0, 0 ) )
                                                ( MT_Vector2D( 0, -10 ) );
    TER_Polygon output;
    TER_Geometry::Scale( output, input, 10.f );
    T_PointVector result = boost::assign::list_of( MT_Vector2D( 0, 20 ) )
                                                 ( MT_Vector2D( -20, 0 ) )
                                                 ( MT_Vector2D( 0, -20 ) )
                                                 ( MT_Vector2D( 20, 0 ) )
                                                 ( MT_Vector2D( 0, 20 ) );
    BOOST_CHECK_EQUAL_COLLECTIONS( output.GetBorderPoints().begin(), output.GetBorderPoints().end(), result.begin(), result.end() );
}
