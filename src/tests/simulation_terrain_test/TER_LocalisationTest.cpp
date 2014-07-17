// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_test_pch.h"
#include "simulation_terrain/TER_Localisation.h"
#include "TER_TestHelpers.h"
#include <tools/BoostTest.h>
#include <tools/Helpers.h>

BOOST_AUTO_TEST_CASE( TER_Localisation_IsEmpty )
{
    const auto empty = LocalisationFromWKT( "POLYGON ()" );
    BOOST_CHECK( empty->IsEmpty() );

    const auto onePoint = LocalisationFromWKT( "POLYGON (0 0)" );
    BOOST_CHECK( onePoint->IsEmpty() );

    const auto twoPoints = LocalisationFromWKT( "POLYGON (0 0, 1 0)" );
    BOOST_CHECK( twoPoints->IsEmpty() );

    const auto invalid = LocalisationFromWKT( "POLYGON (0 0, 1 0, 0 0)" );
    BOOST_CHECK( invalid->IsEmpty() );

    const auto valid = LocalisationFromWKT( "POLYGON (0 0, 1 0, 1 1)" );
    BOOST_CHECK( !valid->IsEmpty() );
}

namespace
{

struct TestIntersecting
{
    std::string wkt;
    bool intersect;
};

} // namespace

BOOST_AUTO_TEST_CASE( TER_Localisation_IsIntersecting )
{
    const std::string otherWkt = "POLYGON (0 0, 3 0, 3 3, 0 3)";
    const auto other = LocalisationFromWKT( otherWkt );

    const TestIntersecting tests[] =
    {
        // Empty
        { "POLYGON ()", false },

        // Invalid
        { "POLYGON (1 1)", false },
        { "POLYGON (1 1, 4 4)", false },
        { "POLYGON (1 1, 4 4, 1 1)", false },

        // Intersecting
        { "POLYGON (1 1, 1 4, 4 4, 4 1)", true },

        // Included
        { "POLYGON (1 1, 2 1, 2 2, 1 2)", true },

        // Disjoint
        { "POLYGON (5 5, 5 6, 6 6, 6 5)", false },
    };

    for( size_t i = 0; i < COUNT_OF( tests ); ++i )
    {
        const auto t = tests[i];
        const auto poly = LocalisationFromWKT( t.wkt );
        if( poly->IsIntersecting( *other ) != t.intersect )
            BOOST_ERROR( "unexpected IsIntersecting between " << t.wkt << " and " << otherWkt );
        if( other->IsIntersecting( *poly ) != t.intersect )
            BOOST_ERROR( "unexpected IsIntersecting between " << otherWkt << " and " << t.wkt );
    }
}

