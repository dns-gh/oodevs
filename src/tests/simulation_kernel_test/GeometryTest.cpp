// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "geometry/Types.h"
#include "simulation_kernel/Tools/MIL_Geometry.h"

// -----------------------------------------------------------------------------
// Name: TestComputePolygonHull

// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestComputePolygonHull )
{
    geometry::Point2f Point1( 0, 0 );
    geometry::Point2f Point2( 0, 10 );
    geometry::Point2f Point3( 10, 10 );
    geometry::Point2f Point4( 10, 0 );
    geometry::Point2f Point5( 10, -10 );
    geometry::Point2f Point6( 0, -10 );
    geometry::Point2f Point7( -10, -10 );
    geometry::Point2f Point8( -10, 0 );
    geometry::Polygon2f input; 
    input.Add( Point1 );
    input.Add( Point2 );
    input.Add( Point3 );
    input.Add( Point4 );
    input.Add( Point5 );
    input.Add( Point6 );
    input.Add( Point7 );
    input.Add( Point8 );
    geometry::Polygon2f output = MIL_Geometry::ComputeHull( input );

    geometry::Polygon2f result; 
    result.Add( Point2 ); 
    result.Add( Point8 ); 
    result.Add( Point7 ); 
    result.Add( Point5 );
    result.Add( Point3 ); 
    BOOST_CHECK_EQUAL( true, output == result );
}

// -----------------------------------------------------------------------------
// Name: TestKnowledgeGroupType
// Created: LDC 2009-12-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestComputePolygonScale )
{
    geometry::Point2f Point1( -10, 0 );
    geometry::Point2f Point2( 0, 10 );
    geometry::Point2f Point3( 10, 0 );
    geometry::Point2f Point4( 0, 0 );
    geometry::Point2f Point5( 0, -10 );
    geometry::Polygon2f input; 
    input.Add( Point1 );
    input.Add( Point2 );
    input.Add( Point3 );
    input.Add( Point4 );
    input.Add( Point5 );
    geometry::Polygon2f output = MIL_Geometry::Scale( input, 10.f );
    geometry::Polygon2f result; 
    geometry::Point2f Point6( -20, 0 );
    geometry::Point2f Point7( 0, 20 );
    geometry::Point2f Point8( 20, 0 );
    geometry::Point2f Point9( 0, -20 );
    result.Add( Point9 ); 
    result.Add( Point8 ); 
    result.Add( Point7 ); 
    result.Add( Point6 ); 
    BOOST_CHECK_EQUAL( true, output == result );
}