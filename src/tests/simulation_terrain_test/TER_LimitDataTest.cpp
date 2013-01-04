// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_test_pch.h"
#include "simulation_terrain/TER_LimitDataManager.h"
#include "simulation_terrain/TER_LimitData.h"

BOOST_AUTO_TEST_CASE( TER_LimitData_Distances )
{
    TER_LimitDataManager m;

    // Test from empty point list
    T_PointVector v1;
    auto p1 = m.CreateLimit( v1 );
    BOOST_CHECK_EQUAL( 0U , p1->GetDistances().size() );
    BOOST_CHECK_EQUAL( 0.0 , p1->GetLength() );

    // Test simple line
    T_PointVector v2;
    v2.push_back( MT_Vector2D( 1.0, 2.0 ));
    v2.push_back( MT_Vector2D( 2.0, 2.0 ));
    v2.push_back( MT_Vector2D( 4.0, 2.0 ));
    v2.push_back( MT_Vector2D( 4.0, 8.0 ));
    auto p2 = m.CreateLimit( v2 );

    const double expected[] = { 1.0, 4.0, 36.0 };
    const size_t expectedLen = sizeof( expected )/sizeof( *expected );
    double length = 0;
    BOOST_REQUIRE_EQUAL( expectedLen, p2->GetDistances().size() );
    for( size_t i = 0; i != expectedLen; ++i )
    {
        BOOST_CHECK_EQUAL( expected[i], p2->GetDistances()[i] );
        length += std::sqrt( expected[i] );
    }
    BOOST_CHECK_EQUAL( length, p2->GetLength() );
}


BOOST_AUTO_TEST_CASE( TER_LimitData_GetDistance )
{
    TER_LimitDataManager m;

    T_PointVector v1;
    v1.push_back( MT_Vector2D( 0.0, 0.0 ));
    v1.push_back( MT_Vector2D( 3.0, 0.0 ));
    v1.push_back( MT_Vector2D( 3.0, 2.0 ));
    auto p1 = m.CreateLimit( v1 );

    // Point projection is before all segments first point
    BOOST_CHECK_EQUAL( 1.0, p1->Distance( MT_Vector2D( -1.0, 0.0 )));
    // Point projection is on a segment
    BOOST_CHECK_EQUAL( 0.5, p1->Distance( MT_Vector2D( 1.5, 0.5 )));
    // Point projection is after all segment second point
    BOOST_CHECK_EQUAL( 1.0, p1->Distance( MT_Vector2D( 3.0, 3.0 )));
}
