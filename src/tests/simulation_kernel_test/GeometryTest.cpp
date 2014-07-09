// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Tools/MIL_Geometry.h"
#include "simulation_kernel/entities/orders/MIL_Fuseau.h"
#include "simulation_kernel/Decision/DEC_GeometryFunctions.h"
#include "simulation_terrain/TER_Polygon.h"
#include "simulation_terrain/TER_Localisation.h"
#include "StubTER_World.h"
#include <boost/assign/list_of.hpp>

// -----------------------------------------------------------------------------
// Name: TestComputePolygonHull

// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestComputePolygonHull )
{
    MT_Vector2D Point1( 0, 0 );
    MT_Vector2D Point2( 0, 10 );
    MT_Vector2D Point3( 10, 10 );
    MT_Vector2D Point4( 10, 0 );
    MT_Vector2D Point5( 10, -10 );
    MT_Vector2D Point6( 0, -10 );
    MT_Vector2D Point7( -10, -10 );
    MT_Vector2D Point8( -10, 0 );
    T_PointVector input;
    input.push_back( Point1 );
    input.push_back( Point2 );
    input.push_back( Point3 );
    input.push_back( Point4 );
    input.push_back( Point5 );
    input.push_back( Point6 );
    input.push_back( Point7 );
    input.push_back( Point8 );
    T_PointVector output;
    MIL_Geometry::ComputeHull( output, input );

    T_PointVector result;
    result.push_back( Point5 );
    result.push_back( Point7 );
    result.push_back( Point8 );
    result.push_back( Point2 );
    result.push_back( Point3 );
    result.push_back( Point5 );
    BOOST_CHECK_EQUAL_COLLECTIONS( output.begin(), output.end(), result.begin(), result.end() );
}

// -----------------------------------------------------------------------------
// Name: TestComputePolygonScale
// Created: ???
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestComputePolygonScale )
{
    T_PointVector input = boost::assign::list_of( MT_Vector2D( -10, 0 ) )
                                                ( MT_Vector2D( 0, 10 ) )
                                                ( MT_Vector2D( 10, 0 ) )
                                                ( MT_Vector2D( 0, 0 ) )
                                                ( MT_Vector2D( 0, -10 ) );
    TER_Polygon output;
    MIL_Geometry::Scale( output, input, 10.f );
    T_PointVector result = boost::assign::list_of( MT_Vector2D( 0, 20 ) )
                                                 ( MT_Vector2D( 20, 0 ) )
                                                 ( MT_Vector2D( 0, -20 ) )
                                                 ( MT_Vector2D( -20, 0 ) )
                                                 ( MT_Vector2D( 0, 20 ) );
    BOOST_CHECK_EQUAL_COLLECTIONS( output.GetBorderPoints().begin(), output.GetBorderPoints().end(), result.begin(), result.end() );
}

BOOST_AUTO_TEST_CASE( AdvanceAlongFuseau )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    {
        MT_Vector2D result;
        MT_Vector2D orientation( 1, 1 );
        std::vector< MT_Vector2D > leftLimit = 
                boost::assign::list_of( MT_Vector2D( 10, 10 ) )
                                      ( MT_Vector2D( 20, 10 ) );
        std::vector< MT_Vector2D > rightLimit = boost::assign::list_of( MT_Vector2D( 10, 20 ) )
                                                                      ( MT_Vector2D( 15, 20 ) )
                                                                      ( MT_Vector2D( 20, 30 ) );
        MIL_Fuseau fuseau( orientation, leftLimit, rightLimit, 0, 0 );
        result = fuseau.GetPositionAtAdvance( 7 );
        BOOST_CHECK_EQUAL( result.rX_, 17 );
        BOOST_CHECK_EQUAL( result.rY_, 15 );
        result = fuseau.GetPositionAtAdvance( -1 );
        BOOST_CHECK_EQUAL( result.rX_, 10 );
        BOOST_CHECK_EQUAL( result.rY_, 15 );
        result = fuseau.GetPositionAtAdvance( 70 );
        BOOST_CHECK_EQUAL( result.rX_, 20 );
        BOOST_CHECK_EQUAL( result.rY_, 20 );
    }
    {
        MT_Vector2D result;
        MT_Vector2D orientation( 1, 1 );
        std::vector< MT_Vector2D > leftLimit = boost::assign::list_of( MT_Vector2D( 100, 100 ) )
                                                                     ( MT_Vector2D( 200, 100 ) );
        std::vector< MT_Vector2D > rightLimit = boost::assign::list_of( MT_Vector2D( 100, 200 ) )
                                                                      ( MT_Vector2D( 150, 200 ) )
                                                                      ( MT_Vector2D( 200, 300 ) );
        MIL_Fuseau fuseau( orientation, leftLimit, rightLimit, 0, 0 );
        result = fuseau.GetPositionAtAdvance( 70 );
        BOOST_CHECK_EQUAL( result.rX_, 170 );
        BOOST_CHECK_EQUAL( result.rY_, 150 );
        result = fuseau.GetPositionAtAdvance( -1 );
        BOOST_CHECK_EQUAL( result.rX_, 100 );
        BOOST_CHECK_EQUAL( result.rY_, 150 );
        result = fuseau.GetPositionAtAdvance( 700 );
        BOOST_CHECK_EQUAL( result.rX_, 200 );
        BOOST_CHECK_EQUAL( result.rY_, 200 );
    }
    {
        MT_Vector2D result;
        MT_Vector2D orientation( 1, 1 );
        std::vector< MT_Vector2D > leftLimit = boost::assign::list_of( MT_Vector2D( 10, 10 ) )
                                                                     ( MT_Vector2D( 30, 10 ) );
        std::vector< MT_Vector2D > rightLimit = boost::assign::list_of( MT_Vector2D( 10, 30 ) )
                                                                      ( MT_Vector2D( 30, 30 ) );
        MIL_Fuseau fuseau( orientation, leftLimit, rightLimit, 0, 0 );
        result = fuseau.GetPositionAtAdvance( 6 );
        BOOST_CHECK_EQUAL( result.rX_, 16 );
        BOOST_CHECK_EQUAL( result.rY_, 20 );
    }
}

BOOST_AUTO_TEST_CASE( GeometryComputeConvexHull )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );

    std::vector< TER_Localisation* > locations;
    locations.push_back( new TER_Localisation( TER_Localisation::ePolygon,
             boost::assign::list_of( MT_Vector2D( 100, 100 ) )
                                   ( MT_Vector2D( 101, 100 ) )
                                   ( MT_Vector2D( 100, 101 ) ) ) );
    locations.push_back( new TER_Localisation( TER_Localisation::eLine,
             boost::assign::list_of( MT_Vector2D( 103, 100 ) )
                                   ( MT_Vector2D( 103, 98 ) ) ) );
    MT_Vector2D mtPoint( 98, 98 );
    std::vector< MT_Vector2D > listPoints;
    listPoints.push_back( mtPoint );
    TER_Localisation* point = new TER_Localisation();
    point->Reset( TER_Localisation::ePoint, listPoints, 1 );
    locations.push_back( point );
    boost::shared_ptr< TER_Localisation > result = DEC_GeometryFunctions::ComputeConvexHull( locations );
    
    T_PointVector expected = boost::assign::list_of( MT_Vector2D( 103, 98 ) )
                                                   ( MT_Vector2D( 99, 97 ) )
                                                   ( MT_Vector2D( 97, 97 ) )
                                                   ( MT_Vector2D( 97, 99 ) )
                                                   ( MT_Vector2D( 100, 101 ) )
                                                   ( MT_Vector2D( 103, 100 ) )
                                                   ( MT_Vector2D( 103, 98 ) );

    BOOST_CHECK_EQUAL_COLLECTIONS( result->GetPoints().begin(), result->GetPoints().end(), expected.begin(), expected.end() );
}
