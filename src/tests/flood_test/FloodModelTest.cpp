// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "flood_test_pch.h"
#include "MockElevationGetter.h"

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : flood_          ( elevationGetter_ )
            , center_         ( 0.f, 0.f )
            , depth_          ( 5 )
            , refDist_        ( 150 )
            , lowElevation_   ( 2u )
            , middleElevation_( 5u )
            , deepElevation_  ( 10u )
        {}

        MockElevationGetter elevationGetter_;
        flood::FloodModel flood_;
        geometry::Point2f center_;
        int depth_;
        int refDist_;
        short lowElevation_;
        short middleElevation_;
        short deepElevation_;
    };
}

// -----------------------------------------------------------------------------
// Name: FloodModel_GenerateSwimmingPool
// Created: ABR 2011-01-11
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( FloodModel_GenerateSwimmingPool, Fixture )
{
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( center_ ).returns( deepElevation_ );

    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f( -100.f, 100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(    0.f, 100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(  100.f, 100.f ) ).returns( deepElevation_ );

    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f( -100.f, 0.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(    0.f, 0.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(  100.f, 0.f ) ).returns( deepElevation_ );

    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f( -100.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(    0.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(  100.f, -100.f ) ).returns( deepElevation_ );

    flood_.GenerateFlood( center_, depth_, refDist_ );

    const flood::FloodModel::T_Polygons& deepArea = flood_.GetDeepAreas();
    const flood::FloodModel::T_Polygons& lowArea = flood_.GetLowAreas();

    BOOST_CHECK_EQUAL( 1u, deepArea.size() );
    BOOST_CHECK_EQUAL( 0u, lowArea.size() );
}

// -----------------------------------------------------------------------------
// Name: FloodModel_GenerateStrips
// Created: ABR 2011-01-11
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( FloodModel_GenerateStrips, Fixture )
{
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( center_ ).returns( middleElevation_ );

    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f( -100.f, 100.f ) ).returns( lowElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(    0.f, 100.f ) ).returns( lowElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(  100.f, 100.f ) ).returns( lowElevation_ );

    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f( -100.f, 0.f ) ).returns( middleElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(    0.f, 0.f ) ).returns( middleElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(  100.f, 0.f ) ).returns( middleElevation_ );

    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f( -100.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(    0.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_, GetElevationAt ).once().with( geometry::Point2f(  100.f, -100.f ) ).returns( deepElevation_ );

    flood_.GenerateFlood( center_, depth_, refDist_ );

    const flood::FloodModel::T_Polygons& deepArea = flood_.GetDeepAreas();
    const flood::FloodModel::T_Polygons& lowArea = flood_.GetLowAreas();

    BOOST_CHECK_EQUAL( 1u, deepArea.size() );
    BOOST_CHECK_EQUAL( 1u, lowArea.size() );
}