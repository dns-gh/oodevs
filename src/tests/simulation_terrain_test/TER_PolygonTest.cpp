// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_test_pch.h"
#include "simulation_terrain/TER_Polygon.h"
#include "TER_TestHelpers.h"
#include <tools/BoostTest.h>
#include <tools/Helpers.h>

BOOST_AUTO_TEST_CASE( TER_Polygon_IsInside )
{
    const auto poly = FromWKT( "POLYGON (0 0, 5 0, 5 5)", false );

    BOOST_CHECK( poly.IsInside( MT_Vector2D( 2, 1 ) ));
    BOOST_CHECK( !poly.IsInside( MT_Vector2D( 6, 6 ) ));

    TER_Polygon empty;
    BOOST_CHECK( empty.IsInside( MT_Vector2D( 6, 6 ) ));
}

namespace
{

struct TestConstructor
{
    std::string wkt;
    bool convexHull;
    std::string expected;
};

} // namespace

BOOST_AUTO_TEST_CASE( TER_Polygon_Constructor )
{
    const TestConstructor tests[] =
    {
        // Empty polygons
        { "POLYGON ()", false, "POLYGON ()" },
        { "POLYGON ()", true, "POLYGON ()" },

        // Invalid polygons
        { "POLYGON (1 1)", false, "POLYGON ()" },
        { "POLYGON (1 1)", true, "POLYGON ()" },

        // Invalid polygons
        { "POLYGON (0 0, 1 1)", false, "POLYGON ()" },
        { "POLYGON (0 0, 1 1)", true, "POLYGON ()" },

        // Flat polygon
        { "POLYGON (0 0, 1 1, 2 2)", false, "POLYGON ()" },
        { "POLYGON (0 0, 1 1, 2 2)", true, "POLYGON ()" },

        // Self-intersecting shapes (bow tie)
        { "POLYGON (0 0, 2 1, 2 0, 0 1, 0 0)", false, "POLYGON (1 0.5, 2 0, 2 1, 1 0.5)" },
        { "POLYGON (0 0, 2 1, 2 0, 0 1, 0 0)", true, "POLYGON (0 0, 2 0, 2 1, 0 1, 0 0)" },

        // Regular concave polygon (right looking rectangular pacman)
        { "POLYGON (0 0, 2 0, 1 1, 2 3, 0 3)", false, "POLYGON (0 0, 2 0, 1 1, 2 3, 0 3, 0 0)" },
        { "POLYGON (0 0, 2 0, 1 1, 2 3, 0 3)", true, "POLYGON (0 0, 2 0, 2 3, 0 3, 0 0)" },
    };

    for( size_t i = 0; i != COUNT_OF( tests ); ++i )
    {
        const auto& t = tests[i];
        auto p = FromWKT( t.wkt, t.convexHull );
        const auto res = ToWKT( p );
        const auto err = tools::CheckOutput( t.expected, res );
        if( !err.empty() )
            BOOST_ERROR( err );
    }
}
