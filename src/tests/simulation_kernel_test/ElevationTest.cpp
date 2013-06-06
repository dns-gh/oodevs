// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Meteo/RawVisionData/Elevation.h"

namespace
{
    struct Fixture
    {
        Fixture()
            : size     ( 2 )
            , elevation( size )
        {}
        void TestAdjacent( const MT_Vector2D& from, const MT_Vector2D& to )
        {
            mock::sequence s;
            MOCK_FUNCTOR( collector, bool( const MT_Vector2D& ) );
            MOCK_EXPECT( collector ).once().with( to ).in( s ).returns( false );
            elevation.FindPath( from, to, collector );
        }
        void TestConsecutive( const MT_Vector2D& from, const MT_Vector2D& middle, const MT_Vector2D& to )
        {
            mock::sequence s;
            MOCK_FUNCTOR( collector, bool( const MT_Vector2D& ) );
            MOCK_EXPECT( collector ).once().with( middle ).in( s ).returns( false );
            MOCK_EXPECT( collector ).once().with( to ).in( s ).returns( false );
            elevation.FindPath( from, to, collector );
        }
        int size;
        Elevation elevation;
    };
}

BOOST_FIXTURE_TEST_CASE( path_from_a_square_to_itself_is_empty, Fixture )
{
    const MT_Vector2D from( 0, 0 );
    const MT_Vector2D to( 0, 1 );
    MOCK_FUNCTOR( collector, bool( const MT_Vector2D& ) );
    elevation.FindPath( from, to, collector );
}

BOOST_FIXTURE_TEST_CASE( path_from_a_square_to_another_adjacent_square_is_made_of_those_two_squares, Fixture )
{
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( 0, size ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( 0, - size ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( size, 0 ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, 0 ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( size, size ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( size, - size ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, size ) );
    TestAdjacent( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, - size ) );
}

BOOST_FIXTURE_TEST_CASE( path_between_consecutive_aligned_squares_is_made_of_all_those_squares, Fixture )
{
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( 0, size ), MT_Vector2D( 0, 2 * size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( 0, - size ), MT_Vector2D( 0, - 2 * size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( size, 0 ), MT_Vector2D( 2 * size, 0 ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, 0 ), MT_Vector2D( - 2 * size, 0 ) );
}

BOOST_FIXTURE_TEST_CASE( path_between_consecutive_non_aligned_squares_is_made_of_all_those_squares, Fixture )
{
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( size, size ), MT_Vector2D( size, 2 * size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( 0, - size ), MT_Vector2D( size, - 2 * size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( size, size ), MT_Vector2D( 2 * size, size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, 0 ), MT_Vector2D( - 2 * size, size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, size ), MT_Vector2D( - size, 2 * size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( 0, - size ), MT_Vector2D( - size, - 2 * size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( size, - size ), MT_Vector2D( 2 * size, - size ) );
    TestConsecutive( MT_Vector2D( 0, 0 ), MT_Vector2D( - size, 0 ), MT_Vector2D( - 2 * size, - size ) );
}
