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
#include "simulation_terrain/TER_World.h"
#include "TER_TestHelpers.h"

BOOST_AUTO_TEST_CASE( TER_Localisation_IsValid )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    const auto empty = LocalisationFromWKT( "POLYGON ()" );
    BOOST_CHECK( !empty->IsValid() );

    const auto onePoint = LocalisationFromWKT( "POLYGON (0 0)" );
    BOOST_CHECK( !onePoint->IsValid() );

    const auto twoPoints = LocalisationFromWKT( "POLYGON (0 0, 1 0)" );
    BOOST_CHECK( !twoPoints->IsValid() );

    const auto invalid = LocalisationFromWKT( "POLYGON (0 0, 1 0, 0 0)" );
    BOOST_CHECK( !invalid->IsValid() );

    const auto valid = LocalisationFromWKT( "POLYGON (0 0, 1 0, 1 1)" );
    BOOST_CHECK( valid->IsValid() );

    const auto emptyCircle = LocalisationFromWKT( "CIRCLE (0 0, 0 0)" );
    BOOST_CHECK( valid->IsValid() );

    const auto validCircle = LocalisationFromWKT( "CIRCLE (0 1, 1 1)" );
    BOOST_CHECK( valid->IsValid() );
}

namespace
{

void CheckIntersecting( std::string wkt1, std::string wkt2, bool intersect )
{
    const auto poly1 = LocalisationFromWKT( wkt1 );
    const auto poly2 = LocalisationFromWKT( wkt2 );
    BOOST_CHECK_MESSAGE( poly1->IsIntersecting( *poly2 ) == intersect,
        "unexpected IsIntersecting between " << wkt1 << " and " << wkt2 );
    BOOST_CHECK_MESSAGE( poly2->IsIntersecting( *poly1 ) == intersect,
        "unexpected IsIntersecting between " << wkt2 << " and " << wkt1 );
}

} // namespace

BOOST_AUTO_TEST_CASE( TER_Localisation_IsIntersecting )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    const std::string wkt1 = "POLYGON (0 0, 3 0, 3 3, 0 3)";

    // Empty and invalid
    CheckIntersecting( wkt1, "POLYGON ()", false );

    // Invalid
    CheckIntersecting( wkt1, "POLYGON (1 1)", false );
    CheckIntersecting( wkt1, "POLYGON (1 1, 4 4)", false );
    CheckIntersecting( wkt1, "POLYGON (1 1, 4 4, 1 1)", false );

    // Intersecting
    CheckIntersecting( wkt1, "POLYGON (1 1, 1 4, 4 4, 4 1)", true );

    // Included
    CheckIntersecting( wkt1, "POLYGON (1 1, 2 1, 2 2, 1 2)", true );

    // Disjoint
    CheckIntersecting( wkt1, "POLYGON (5 5, 5 6, 6 6, 6 5)", false );
}

