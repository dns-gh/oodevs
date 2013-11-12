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

BOOST_AUTO_TEST_CASE( TER_Polygon_IsInside )
{
    T_PointVector v;
    v.push_back( MT_Vector2D( 0.0, 0.0 ));
    v.push_back( MT_Vector2D( 5.0, 0.0 ));
    v.push_back( MT_Vector2D( 5.0, 5.0 ));
    TER_Polygon poly;
    poly.Reset( v );

    BOOST_CHECK( poly.IsInside( MT_Vector2D( 2, 1 ) ));
    BOOST_CHECK( !poly.IsInside( MT_Vector2D( 6, 6 ) ));

    TER_Polygon empty;
    BOOST_CHECK( empty.IsInside( MT_Vector2D( 6, 6 ) ));
}

