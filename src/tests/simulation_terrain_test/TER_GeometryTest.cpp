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

BOOST_AUTO_TEST_CASE( TestComputeHull )
{
    // Cases for more than two points or non-colinear points are already
    // covered by TER_Polygon
    auto points = boost::assign::list_of
        ( MT_Vector2D( 0, 0 ) )
        ( MT_Vector2D( 1, 0 ) )
        ( MT_Vector2D( 2, 0 ) )
        ;
    for( auto it = points.begin(); it != points.end(); ++it )
    {
        T_PointVector inputs( points.begin(), it );
        T_PointVector hull;
        TER_Geometry::ComputeHull( hull, inputs );
        BOOST_CHECK_EQUAL_COLLECTIONS( hull.begin(), hull.end(), inputs.begin(), inputs.end() );
    }
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
