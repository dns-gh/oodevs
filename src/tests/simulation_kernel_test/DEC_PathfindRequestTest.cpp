// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Decision/DEC_PathFindRequest.h"
#include "simulation_kernel/Decision/DEC_PathFind_Manager_ABC.h"
#include "simulation_kernel/Decision/DEC_PathComputer_ABC.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include "simulation_terrain/TER_World_ABC.h"
#include <pathfind/TerrainPathPoint.h>
#include <pathfind/TerrainRule_ABC.h>
#include <tools/thread/Handler_ABC.h>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

namespace
{
    MOCK_BASE_CLASS( MockDEC_Pathfind_Manager, DEC_PathFind_Manager_ABC )
    {
        MOCK_METHOD( Update, 0 )
        MOCK_METHOD( UpdateInSimulationThread, 0 )
        MOCK_METHOD( StartCompute, 2 )
        MOCK_METHOD( CancelJobForUnit, 1 )
        MOCK_METHOD( CleanPathAfterComputation, 1 )
        MOCK_METHOD( GetCurrentThread, 0 )
        MOCK_METHOD( GetNbrShortRequests, 0 )
        MOCK_METHOD( GetNbrLongRequests, 0 )
        MOCK_METHOD( GetNbrTreatedRequests, 0 )
        MOCK_METHOD( GetMaxComputationDuration, 0 )
    };

    MOCK_BASE_CLASS( MockDEC_PathComputer, DEC_PathComputer_ABC )
    {
        MOCK_METHOD( AddResultPoint, 4 )
        MOCK_METHOD( GetLength, 0 )
        MOCK_METHOD( Execute, 1 )
        MOCK_METHOD( Cancel, 0 )
        MOCK_METHOD( GetState, 0 )
        MOCK_METHOD( RegisterPathSection, 1 )
        MOCK_METHOD( GetLastWaypoint, 0 )
        MOCK_METHOD( GetComputedWaypoints, 0 )
        MOCK_METHOD( RemoveComputedWaypoint, 0 )
        MOCK_METHOD( GetResult, 0 )
    };

    MOCK_BASE_CLASS( MockTER_Pathfinder, TER_Pathfinder_ABC )
    {
        MOCK_METHOD( SetId, 1 )
        MOCK_METHOD( SetCallback, 1 )
        MOCK_METHOD( SetChoiceRatio, 1 )
        MOCK_METHOD( SetConfiguration, 2 )
        MOCK_METHOD( ComputePath, 4 )
    };

    MOCK_BASE_CLASS( MockTerrainRule, TerrainRule_ABC )
    {
        MOCK_METHOD( EvaluateCost, 2 )
        MOCK_METHOD( GetCost, 5 )
    };

    MOCK_BASE_CLASS( MockHandler, tools::thread::Handler_ABC< TerrainPathPoint > )
    {
        MOCK_METHOD( Handle, 1 )
    };

    MOCK_BASE_CLASS( MockWorld, TER_World_ABC )
    {
        MOCK_METHOD( MosToSimMgrsCoord, 2, void( const std::string&, MT_Vector2D& ), MosToSimMgrsCoord_Text )
        MOCK_METHOD( SimToMosMgrsCoord, 2, void( const MT_Vector2D&, std::string& ), SimToMosMgrsCoord_Text )
        MOCK_METHOD( MosToSimMgrsCoord, 3, void( double, double, MT_Vector2D& ), MosToSimMgrsCoord_LatLong )
        MOCK_METHOD( SimToMosMgrsCoord, 3, void( const MT_Vector2D&, double&, double& ), SimToMosMgrsCoord_LatLong )
        MOCK_METHOD( GetWeldValue, 0 )
        MOCK_METHOD( GetWidth, 0 )
        MOCK_METHOD( GetHeight, 0 )
        MOCK_METHOD( GetExtent, 0 )
        MOCK_METHOD( IsValidPosition, 1 )
        MOCK_METHOD( ClipPointInsideWorld, 1, void( MT_Vector2D& ), ClipPointInsideWorld_NotConst )
        MOCK_METHOD( ClipPointInsideWorld, 1, MT_Vector2D( const MT_Vector2D& ), ClipPointInsideWorld_Const )
    };

    void Assign( double latitude, double longitude, MT_Vector2D& point )
    {
        point.rX_ = longitude;
        point.rY_ = latitude;
    }

    void AddPoint( const geometry::Point2f& point, int waypoint, sword::PathResult& result )
    {
        auto position = result.add_points();
        position->mutable_coordinate()->set_longitude( point.X() );
        position->mutable_coordinate()->set_latitude( point.Y() );
        position->set_waypoint( waypoint );
    }

    bool CheckPathPoint( const TerrainPathPoint actual, const geometry::Point2f& expected )
    {
        return actual.Point() == expected;
    }

    class Fixture
    {
    public:
        Fixture()
            : computer( new MockDEC_PathComputer() )
            , from( 0, 0 )
            , to( 1, 1 )
        {
            MOCK_EXPECT( manager.CleanPathAfterComputation ).once();
            MOCK_EXPECT( world.MosToSimMgrsCoord_LatLong ).calls( boost::bind( &Assign, _1, _2, _3 ) );
        }

        void AddCheckPoints( const std::vector< std::pair< geometry::Point2f, int > >& points )
        {
            mock::sequence s;
            auto& result = *pathfind.mutable_result();
            BOOST_FOREACH( const auto& point, points )
            {
                AddPoint( point.first, point.second, result );
                MOCK_EXPECT( handler.Handle ).once().in( s ).with( boost::bind( &CheckPathPoint, _1, point.first ) );
            }
        }
        void ExecutePathfind()
        {
            DEC_PathFindRequest request( manager, computer, pathfind, world );
            MOCK_EXPECT( computer->Execute ).once().calls( boost::bind( &TER_Pathfinder_ABC::ComputePath, _1, from, to, rule, handler ) );
            request.FindPath( realPathfinder );
        }
        MockDEC_Pathfind_Manager manager;
        boost::shared_ptr< MockDEC_PathComputer > computer;
        sword::Pathfind pathfind;
        MockWorld world;
        MockTER_Pathfinder realPathfinder;
        MockTerrainRule rule;
        MockHandler handler;
        const geometry::Point2f from;
        const geometry::Point2f to;
   };
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_without_result_starts_a_real_computation, Fixture )
{
    MOCK_EXPECT( realPathfinder.ComputePath ).once().returns( true );
    ExecutePathfind();
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_with_matching_precomputed_pathfind_uses_it, Fixture )
{
    AddCheckPoints( boost::assign::map_list_of( from, 0 )
                                              ( geometry::Point2f( 0.5, 0.5 ), -1 )
                                              ( to, 1 ) );
    ExecutePathfind();
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_matches_any_segment_in_result, Fixture )
{
    AddPoint( geometry::Point2f( -1, -1 ), 0, *pathfind.mutable_result() );
    AddCheckPoints( boost::assign::map_list_of( from, 1 )
                                              ( to, 2 ) );
    ExecutePathfind();
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_matches_segment_with_waypoints, Fixture )
{
    AddPoint( from, 0, *pathfind.mutable_result() );
    AddPoint( geometry::Point2f( 0.5, 0.5 ), 1, *pathfind.mutable_result() );
    AddPoint( to, 2, *pathfind.mutable_result() );
    MOCK_EXPECT( realPathfinder.ComputePath ).once().returns( true );
    ExecutePathfind();
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_can_match_same_waypoint_in_multiple_segments, Fixture )
{
    AddPoint( from, 0, *pathfind.mutable_result() );
    AddPoint( geometry::Point2f( 0.5, 0.5 ), 1, *pathfind.mutable_result() );
    AddPoint( to, 2, *pathfind.mutable_result() );
    AddCheckPoints( boost::assign::map_list_of( from, 3 )
                                              ( geometry::Point2f( 2, 2 ), -1 )
                                              ( to, 4 ) );
    ExecutePathfind();
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_can_match_same_waypoint_in_source_and_destination, Fixture )
{
    AddCheckPoints( boost::assign::map_list_of( from, 0 )
                                              ( geometry::Point2f( 2, 2 ), -1 )
                                              ( from, 1 ) );
    DEC_PathFindRequest request( manager, computer, pathfind, world );
    MOCK_EXPECT( computer->Execute ).once().calls( boost::bind( &TER_Pathfinder_ABC::ComputePath, _1, from, from, rule, handler ) );
    request.FindPath( realPathfinder );
}

BOOST_FIXTURE_TEST_CASE( pathfind_request_with_result_but_unmatching_segment_starts_a_real_computation, Fixture )
{
    AddPoint( from, 0, *pathfind.mutable_result() );
    AddPoint( to, 1, *pathfind.mutable_result() );
    DEC_PathFindRequest request( manager, computer, pathfind, world );
    MOCK_EXPECT( realPathfinder.ComputePath ).once().returns( true );
    MOCK_EXPECT( computer->Execute ).once().calls( boost::bind( &TER_Pathfinder_ABC::ComputePath, _1, to, from, rule, handler ) );
    request.FindPath( realPathfinder );
}
