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
#include <boost/bind/apply.hpp>

using namespace sword::movement;

namespace
{
    struct MovementFixture : public PathfindFixture
    {
        MovementFixture()
            : action( 117u )
            , command( 0u )
        {}
        virtual ~MovementFixture()
        {
            StopCommand( command );
        }
        size_t StartMoveCommand( const T_Points& points )
        {
            ConfigurePathfind( points );
            boost::shared_ptr< sword::movement::Path_ABC > path = CreatePathParameter( points.back().first );
            return StartCommand( "move",
                core::MakeModel( "action", action )
                               ( "identifier", identifier )
                               ( "path", core::MakeUserData( path ) ) );
        }
        void ConfigurePathfind( const T_Points& points )
        {
            MOCK_EXPECT( UsePathDebug ).returns( false );
            MOCK_EXPECT( IsDestinationTrafficable ).returns( true );
            MOCK_EXPECT( GetMaxPathFindComputationDuration ).returns( std::numeric_limits< unsigned int >::max() );
            MOCK_EXPECT( IsNullAutomateFuseau ).returns( true );
            MOCK_EXPECT( ComputePathfind ).once().calls( bp::bind( &PathfindFixture::AddPoints, this, boost::ref( points ), bp::arg_names::arg10, bp::arg_names::arg11 ) );
            MOCK_EXPECT( StartComputePathfind ).once().calls( boost::bind( &ExecutePathfind, _2, boost::ref( pathfind ) ) );
        }
        void Step( double currentSpeed, bool hasResources, bool canMove )
        {
            const bool obstaclesChange = false;
            entity[ "movement/has-resources" ] = hasResources;
            entity[ "movement/can-move" ] = canMove;
            MOCK_EXPECT( GetWorldWeldValue ).returns( 1000 );
            MOCK_EXPECT( GetLimas );
            MOCK_EXPECT( GetSpeedWithReinforcement ).returns( currentSpeed );
            MOCK_EXPECT( GetObjectListWithinCircle );
            MOCK_EXPECT( UpdateObjectsToAvoid ).returns( obstaclesChange );
            ExecuteCommands();
            mock::verify();
            mock::reset();
        }
        void Advance( float currentSpeed, unsigned int code, bool hasResources, bool canMove )
        {
            ExpectEffect( entity[ "movement" ] );
            ExpectCallbackEvent( code );
            ExpectMovementEvent( currentSpeed );
            Step( currentSpeed, hasResources, canMove );
            UpdatePosition( geometry::Point2f( 0, entity[ "movement/position/y" ] + currentSpeed ) );
        }
        void ExpectCallbackEvent( unsigned int code )
        {
            ExpectEvent( "movement callback", sword::test::MakeModel( "code", code )
                                                                    ( "entity", identifier )
                                                                    ( "action", action ) );
        }
        void ExpectMovementEvent( double speed )
        {
            ExpectEvent( "movement", sword::test::MakeModel( "distance", speed )
                                                           ( "entity", identifier ) );
        }
        size_t action;
        size_t command;
    };
}

BOOST_FIXTURE_TEST_CASE( movement_command_sends_movement_environment_effect_when_changing_terrain_type, MovementFixture )
{
    const T_Points points = boost::assign::map_list_of( geometry::Point2f( 0, 0 ), TerrainData() )
                                                      ( geometry::Point2f( 0, 1 ), TerrainData() )
                                                      ( geometry::Point2f( 0, 5 ), TerrainData::Water() )
                                                      ( geometry::Point2f( 0, 10 ), TerrainData() );
    UpdatePosition( points.front().first );
    command = StartMoveCommand( points );
    ExpectEffect( entity[ "movement/environment" ], sword::test::MakeModel( "area", TerrainData::Water().Area() )
                                                                          ( "left", 0 )
                                                                          ( "right", 0 )
                                                                          ( "linear", 0 ) );
    ExpectEffect( entity[ "movement/path" ] );
    MOCK_EXPECT( NotifyMovingOnPathPoint ).once();
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    MOCK_EXPECT( NotifyMovingOnPathPoint ).once();
    MOCK_EXPECT( GetSpeedWithReinforcement ).once().returns( 4 );
    ExpectEffect( entity[ "movement/environment" ], sword::test::MakeModel( "area", TerrainData().Area() )
                                                                          ( "left", 0 )
                                                                          ( "right", 0 )
                                                                          ( "linear", 0 ) );
    Advance( 4, sword::movement::PathWalker::eRunning, true, true );
    MOCK_EXPECT( NotifyMovingOnPathPoint ).once();
    Advance( 5, sword::movement::PathWalker::eFinished, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( moving_on_canceled_path_sends_not_allowed_callback_and_does_not_move, MovementFixture )
{
    MOCK_EXPECT( IsDestinationTrafficable ).returns( false );
    ExpectEvent( "movement report" );
    boost::shared_ptr< sword::movement::Path_ABC > path = CreateSimplePath();
    command = StartCommand( "move",
        core::MakeModel( "action", action )
            ( "identifier", identifier )
            ( "path", core::MakeUserData( path ) ) );
    mock::verify();
    mock::reset();
    ExpectCallbackEvent( sword::movement::PathWalker::eNotAllowed );
    Step( 1, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( moving_on_impossible_path_sends_not_allowed_callback_and_does_not_move, MovementFixture )
{
    ConfigureImpossiblePathfind();
    ExpectEvent( "movement report" );
    boost::shared_ptr< sword::movement::Path_ABC > path = CreateSimplePath();
    command = StartCommand( "move",
        core::MakeModel( "action", action )
            ( "identifier", identifier )
            ( "path", core::MakeUserData( path ) ) );
    mock::verify();
    mock::reset();
    ExpectCallbackEvent( sword::movement::PathWalker::eNotAllowed );
    Step( 1, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

namespace
{
    struct StartedFixture : public MovementFixture
    {
        StartedFixture()
        {
            const geometry::Point2f start( 0, 0 );
            const geometry::Point2f end( 0, 10 );
            UpdatePosition( start );
            const T_Points points = boost::assign::map_list_of( start, TerrainData() )
                                                              ( end, TerrainData() );
            command = StartMoveCommand( points );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( movement_command_in_paused_execution_only_sends_paused_event, StartedFixture )
{
    commands.Pause( command );
    ExpectCallbackEvent( sword::movement::PathWalker::ePaused );
    ExecuteCommands();
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( first_movement_command_posts_path_effect_and_running_code_event_if_not_arrived, StartedFixture )
{
    const double currentSpeed = 5;
    ExpectEffect( entity[ "movement" ], sword::test::MakeModel( "direction/x", 0 )
                                                              ( "direction/y", 1 )
                                                              ( "position/x", 0 )
                                                              ( "position/y", currentSpeed )
                                                              ( "speed", currentSpeed ) );
    ExpectEffect( entity[ "movement/path" ], sword::test::MakeModel( "identifier", 1 )
        ( "points", sword::test::MakeModel()[ sword::test::MakeModel( "x", 0 )( "y", 0 ) ]
                                            [ sword::test::MakeModel( "x", 0 )( "y", 10 ) ] ) );
    ExpectCallbackEvent( sword::movement::PathWalker::eRunning );
    ExpectMovementEvent( currentSpeed );
    Step( currentSpeed, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( movement_command_posts_finished_code_event_if_arrived, StartedFixture )
{
    MOCK_EXPECT( NotifyMovingOnPathPoint ).once();
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 10, sword::movement::PathWalker::eFinished, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( movement_command_advance_on_path, StartedFixture )
{
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 5, sword::movement::PathWalker::eRunning, true, true );
    MOCK_EXPECT( NotifyMovingOnPathPoint ).once();
    Advance( 5, sword::movement::PathWalker::eFinished, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( stopping_command_cancels_pathfind_job, StartedFixture )
{
    MOCK_EXPECT( CancelPathFindJob ).once();
    commands.Stop( command );
    mock::verify();
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( movement_without_resources_sends_out_of_gas_report_and_not_enough_fuel_callback, StartedFixture )
{
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    ExpectEvent( "movement report", sword::test::MakeModel( "entity/data", "data" )
                                                          ( "code", static_cast< int >( MIL_Report::eReport_OutOfGas ) ) );
    Advance( 1, sword::movement::PathWalker::eNotEnoughFuel, false, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( ouf_of_gas_report_is_only_sent_once, StartedFixture )
{
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    ExpectEvent( "movement report" );
    Advance( 1, sword::movement::PathWalker::eNotEnoughFuel, false, true );
    Advance( 1, sword::movement::PathWalker::eNotEnoughFuel, false, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( movement_with_new_resources_moves_again, StartedFixture )
{
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    ExpectEvent( "movement report" );
    Advance( 1, sword::movement::PathWalker::eNotEnoughFuel, false, true );
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

BOOST_FIXTURE_TEST_CASE( movement_with_entity_that_cannot_move_sends_not_allowed_callback_and_sets_speed_at_0, StartedFixture )
{
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    const double currentSpeed = 1;
    const double stopSpeed = 0;
    ExpectEffect( entity[ "movement" ], sword::test::MakeModel( "direction/x", 0 )
                                                              ( "direction/y", 0 )
                                                              ( "position/x", 0 )
                                                              ( "position/y", 1 )
                                                              ( "speed", stopSpeed ) );
    ExpectCallbackEvent( sword::movement::PathWalker::eNotAllowed );
    ExpectMovementEvent( currentSpeed );
    Step( currentSpeed, true, false );
    MOCK_EXPECT( CancelPathFindJob ).once();
}

namespace
{
    struct ObjectFixture : public StartedFixture
    {
        ObjectFixture()
            : weldValue( 1000 )
        {
            MOCK_RESET( GetObjectListWithinCircle );
        }
        template< typename Callback >
        bool AddCollision( Callback callback, void* userData )
        {
            MT_Vector2D collision;
            callback( collision, userData );
            return true;
        }
        void AddObjectOnPath()
        {
            object = 42;
            MOCK_EXPECT( GetObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, core::Convert( &object ), _5 ) );
            MOCK_EXPECT( ObjectIntersect2D ).once().calls( boost::bind( &ObjectFixture::AddCollision< void(*)( const MT_Vector2D&, void* ) >, this, _3, _4 ) );
            MOCK_EXPECT( ObjectIsInside ).once().returns( true );
        }
        const double weldValue;
        core::Model object;
    };
}

BOOST_FIXTURE_TEST_CASE( object_colliding_with_path_makes_command_send_blocked_by_object_callback, ObjectFixture )
{
    AddObjectOnPath();
    MOCK_EXPECT( CanObjectInteractWith ).once().returns( true );
    MOCK_EXPECT( GetSpeedWithReinforcementObject ).once().returns( 0. );
    MOCK_EXPECT( ObjectIsInside ).returns( false );
    MOCK_EXPECT( NotifyMovingOutsideObject ).once();
    ExpectEffect( entity[ "movement/path" ] );
    Advance( 1, sword::movement::PathWalker::eRunning, true, true );
    mock::verify();
    AddObjectOnPath();
    MOCK_EXPECT( CanObjectInteractWith ).returns( true );
    MOCK_EXPECT( NotifyMovingInsideObject ).once();
    MOCK_EXPECT( GetSpeedWithReinforcementObject ).once().returns( 0. );
    MOCK_EXPECT( ObjectIsInside ).returns( false );
    MOCK_EXPECT( ObjectIsOnBorder ).returns( false );
    MOCK_EXPECT( NotifyMovingOutsideObject ).once();
    ExpectEffect( entity[ "movement" ] );
    ExpectCallbackEvent( sword::movement::PathWalker::eBlockedByObject );
    ExpectMovementEvent( weldValue + 1 );
    Step( 2, true, true );
    MOCK_EXPECT( CancelPathFindJob ).once();
}
