// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "propagation_test_pch.h"
#include "propagation/FloodModel.h"
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
        propagation::FloodModel flood_;
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
    MOCK_EXPECT( elevationGetter_.GetCellSize ).returns( 100.f );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( center_ ).returns( deepElevation_ );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f( -100.f, 100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(    0.f, 100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(  100.f, 100.f ) ).returns( deepElevation_ );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f( -100.f, 0.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(    0.f, 0.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(  100.f, 0.f ) ).returns( deepElevation_ );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f( -100.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(    0.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(  100.f, -100.f ) ).returns( deepElevation_ );

    propagation::FloodModel_ABC::T_Polygons deepArea;
    propagation::FloodModel_ABC::T_Polygons lowArea;

    flood_.GenerateFlood( center_, deepArea, lowArea, depth_, refDist_ );

    BOOST_CHECK_EQUAL( 1u, deepArea.size() );
    BOOST_CHECK_EQUAL( 0u, lowArea.size() );
}

// -----------------------------------------------------------------------------
// Name: FloodModel_GenerateStrips
// Created: ABR 2011-01-11
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( FloodModel_GenerateStrips, Fixture )
{
    MOCK_EXPECT( elevationGetter_.GetCellSize ).returns( 100.f );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( center_ ).returns( middleElevation_ );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f( -100.f, 100.f ) ).returns( lowElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(    0.f, 100.f ) ).returns( lowElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(  100.f, 100.f ) ).returns( lowElevation_ );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f( -100.f, 0.f ) ).returns( middleElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(    0.f, 0.f ) ).returns( middleElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(  100.f, 0.f ) ).returns( middleElevation_ );

    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f( -100.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(    0.f, -100.f ) ).returns( deepElevation_ );
    MOCK_EXPECT( elevationGetter_.GetElevationAt ).once().with( geometry::Point2f(  100.f, -100.f ) ).returns( deepElevation_ );

    propagation::FloodModel_ABC::T_Polygons deepArea;
    propagation::FloodModel_ABC::T_Polygons lowArea;

    flood_.GenerateFlood( center_, deepArea, lowArea, depth_, refDist_ );

    BOOST_CHECK_EQUAL( 1u, deepArea.size() );
    BOOST_CHECK_EQUAL( 1u, lowArea.size() );
}
