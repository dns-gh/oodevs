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
#include <tools/Helpers.h>
#include <tools/BoostTest.h>
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

namespace
{

struct TestBuffer
{
    T_PointVector points;
    std::string wkt;
};

}  // namespace

BOOST_AUTO_TEST_CASE( TestTerGeometryBuffer )
{
    const TestBuffer tests[] =
    {
        // A point generates an octogon
        { boost::assign::list_of( MT_Vector2D( 0, 0 ) ),
            "POLYGON (1 0, 0.707107 0.707107, -4.62459e-015 1, -0.707107 0.707107, "
               "-1 -3.23109e-015, -0.707107 -0.707107, 1.61554e-015 -1, 0.707107 -0.707107, 1 0)" },

        // A line generates two half octogons and a rectangle in-between
        { boost::assign::list_of
            ( MT_Vector2D( 0, 0 ) )
            ( MT_Vector2D( 1, 0 ) ),
               "POLYGON (1 1, 0 1, -0.707107 0.707107, -1 1.22465e-016, "
                   "-0.707107 -0.707107, 0 -1, 1 -1, 1.70711 -0.707107, "
                   "2 0, 1.70711 0.707107, 1 1)" },

        // Right looking rectangular pacman, gives a box with rounded corner
        // because the convex hull is applied before the buffer
        { boost::assign::list_of
            ( MT_Vector2D( 0, 0 ) )
            ( MT_Vector2D( 2, 0 ) )
            ( MT_Vector2D( 1, 1 ) )
            ( MT_Vector2D( 2, 2 ) )
            ( MT_Vector2D( 0, 2 ) ),
                "POLYGON (0 -1, 2 -1, 2.70711 -0.707107, 3 0, 3 2, 2.70711 2.70711, "
                    "2 3, 0 3, -0.707107 2.70711, -1 2, -1 0, -0.707107 -0.707107, 0 -1)" },
    };

    for( size_t i = 0; i < COUNT_OF( tests ); ++i )
    {
        const auto& t = tests[i];
        TER_Polygon output;
        TER_Geometry::Buffer( output, t.points, 1 );
        const auto res = ToWKT( output );
        const auto err = tools::CheckOutput( res, t.wkt );
        if( !err.empty() )
            BOOST_ERROR( err );
    }
}
