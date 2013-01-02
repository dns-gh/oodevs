// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "movement_module_test_pch.h"
#include "PathfindFixture.h"
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <boost/thread.hpp>
#pragma warning( pop )

using namespace sword::movement;

BOOST_FIXTURE_TEST_CASE( destination_not_trafficable_cancels_path_computation_and_sends_difficult_terrain_report_event, PathfindFixture ) // $$$$ _RC_ SLI 2012-03-09: event is sent outside command
{
    ExpectEvent( "movement report", sword::test::MakeModel( "entity/data", "data" )
                                                          ( "code", static_cast< int >( MIL_Report::eRC_TerrainDifficile ) ) );
    MOCK_EXPECT( IsDestinationTrafficable ).returns( false );
    CreateSimplePath();
}

BOOST_FIXTURE_TEST_CASE( impossible_path_sends_difficult_terrain_report_event, PathfindFixture ) // $$$$ _RC_ SLI 2012-03-09: event is sent outside command
{
    ExpectEvent( "movement report", sword::test::MakeModel( "entity/data", "data" )
                                                          ( "code", static_cast< int >( MIL_Report::eRC_TerrainDifficile ) ) );
    ConfigureImpossiblePathfind();
    CreateSimplePath();
}

namespace
{
    bool CheckShouldEndComputation( PathfindShouldEndComputation termination, void* terminationData )
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds( 1u ) ); // $$$$ _RC_ SLI 2012-03-09: refactor Path section using time() function
        return termination( 0, 0, terminationData );
    }
}

BOOST_FIXTURE_TEST_CASE( path_too_long_to_compute_sends_timeout_log, PathfindFixture )
{
    TER_Pathfinder_ABC pathfind;
    MOCK_EXPECT( UsePathDebug ).returns( false );
    MOCK_EXPECT( IsDestinationTrafficable ).once().returns( true );
    MOCK_EXPECT( GetMaxPathFindComputationDuration ).once().returns( 0 );
    MOCK_EXPECT( IsNullAutomateFuseau ).once().returns( true );
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, "Pathfind computation aborted - timeout" );
    MOCK_EXPECT( ComputePathfind ).once().calls( bp::bind( &CheckShouldEndComputation, bp::arg_names::arg12, bp::arg_names::arg13 ) );
    MOCK_EXPECT( StartComputePathfind ).once().calls( boost::bind( &ExecutePathfind, _1, boost::ref( pathfind ) ) );
    CreateSimplePath();
}
