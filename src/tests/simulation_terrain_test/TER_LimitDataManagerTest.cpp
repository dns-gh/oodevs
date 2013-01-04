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

BOOST_AUTO_TEST_CASE( TER_LimitDataLifecycle )
{
    T_PointVector v1;
    v1.push_back( MT_Vector2D( 1.0, 1.0 ));
    v1.push_back( MT_Vector2D( 2.0, 1.0 ));

    T_PointVector v2;
    v2.push_back( MT_Vector2D( 1.0, 2.0 ));
    v2.push_back( MT_Vector2D( 2.0, 2.0 ));

    T_PointVector v3 = v2;

    // Test regular limit creation
    TER_LimitDataManager m;
    auto p1 = m.CreateLimit( v1 );
    BOOST_CHECK( v1 == p1->GetPoints() );

    auto p2 = m.CreateLimit( v2 );
    BOOST_CHECK( v2 == p2->GetPoints() );

    // Test deduplication on input points data (and not instance)
    auto p3 = m.CreateLimit( v3 );
    BOOST_CHECK( v3 == p3->GetPoints() );
    BOOST_CHECK_EQUAL( p2, p3 );

    // Test lifetime management logic
    BOOST_CHECK( m.HasLimit( v2 ));
    p2.reset();
    BOOST_CHECK( m.HasLimit( v2 ));
    p3.reset();
    BOOST_CHECK( !m.HasLimit( v2 ));
}
