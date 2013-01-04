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

    // Test simple line
    T_PointVector v2;
    v2.push_back( MT_Vector2D( 1.0, 2.0 ));
    v2.push_back( MT_Vector2D( 2.0, 2.0 ));
    v2.push_back( MT_Vector2D( 4.0, 2.0 ));
    v2.push_back( MT_Vector2D( 4.0, 8.0 ));
    auto p2 = m.CreateLimit( v2 );

    const double expected[] = { 1.0, 4.0, 36.0 };
    const size_t expectedLen = sizeof( expected )/sizeof( *expected );
    BOOST_REQUIRE_EQUAL( expectedLen, p2->GetDistances().size() );
    for( size_t i = 0; i != expectedLen; ++i )
    {
        BOOST_CHECK_EQUAL( expected[i], p2->GetDistances()[i].SquareLength() );
    }
}
