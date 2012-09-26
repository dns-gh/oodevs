// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "PerceptionCommandFixture.h"

using namespace sword::perception;

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_at_least_itself, PerceptionCommandFixture )
{
    ExpectEffect( entity[ "perceptions/notifications/objects" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/population-flows" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/population-concentrations" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/urban-blocks" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/agents" ], sword::test::MakeModel()
                                                                 [ sword::test::MakeModel( "level", 3 )
                                                                                         ( "target", 1337 )
                                                                                         ( "recorded", false ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_its_transporter, PerceptionCommandFixture )
{
    const size_t transporterId = 1342;
    const SWORD_Model* transporter = core::Convert( &model[ "entities" ][ transporterId ] );
    model[ "entities" ][ transporterId ][ "pion" ] = 1818;
    MOCK_RESET( GetTransporter );
    MOCK_EXPECT( GetTransporter ).returns( transporter );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( mock::any ) ]
                                       [ sword::test::MakeModel( "level", 3 )
                                                               ( "target", 1818 )
                                                               ( "recorded", false ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_urban_objects_in_list, PerceptionCommandFixture )
{
    const SWORD_Model* urbanObject = core::Convert( &model[ "objects/urban-object" ] );
    model[ "objects/urban-object" ] = core::MakeModel( "data", 42 );
    MOCK_RESET( GetUrbanObjectListWithinCircle );
    MOCK_EXPECT( GetUrbanObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, urbanObject, _5 ) );
    MOCK_EXPECT( GetUrbanObjectOccupation ).once().with( urbanObject ).returns( 1 );
    ExpectNotifications( "urban-blocks", sword::test::MakeModel()
                                             [ sword::test::MakeModel( "level", 3 )
                                                                     ( "target/data", 42 ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_command_updates_next_peripherical_vision_step, PerceptionCommandFixture )
{
    ExpectNotifications();
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
    mock::verify();
    model[ "tick" ] = 1;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ], sword::test::MakeModel( "next-tick", 13 )
                                                                                     ( "activated", true ) );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ] );
    ExpectEffect( entity[ "perceptions/cones" ] );
    ExpectNotifications();
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_command_with_direction_vision_uses_movement_direction_as_main_perception_direction, PerceptionCommandFixture )
{
    ExpectNotifications();
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
    mock::verify();
    model[ "tick" ] = 1;
    entity[ "perceptions/vision/mode" ] = "direction";
    entity[ "perceptions/vision/location/x" ] = 42.;
    entity[ "perceptions/vision/location/y" ] = 43.;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", 42. )
                                                                                           ( "y", 43. ) );
    ExpectEffect( entity[ "perceptions/cones" ] );
    ExpectNotifications();
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_command_with_location_vision_mode_computes_location_direction_as_main_perception_direction, PerceptionCommandFixture )
{
    ExpectNotifications();
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
    mock::verify();
    model[ "tick" ] = 1;
    entity[ "perceptions/vision/mode" ] = "location";
    entity[ "perceptions/vision/location/x" ] = 0;
    entity[ "perceptions/vision/location/y" ] = 0;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", 0 )
                                                                                           ( "y", -1 ) );
    ExpectEffect( entity[ "perceptions/cones" ] );
    ExpectNotifications();
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_command_updates_vision_cones, PerceptionViewFixture )
{
    MOCK_EXPECT( GetUrbanObjectListWithinCircle );
    MOCK_EXPECT( AppendAddedKnowledge );
    MOCK_EXPECT( GetAgentListWithinCircle );
    MOCK_EXPECT( GetObjectListWithinCircle );
    MOCK_EXPECT( GetConcentrationListWithinCircle );
    MOCK_EXPECT( GetFlowListWithinCircle );
    MOCK_EXPECT( GetTransporter ).returns( 0 );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ] );
    ExpectEffect( entity[ "perceptions/notifications/agents" ] );
    ExpectEffect( entity[ "perceptions/notifications/objects" ] );
    ExpectEffect( entity[ "perceptions/notifications/population-flows" ] );
    ExpectEffect( entity[ "perceptions/notifications/population-concentrations" ] );
    ExpectEffect( entity[ "perceptions/notifications/urban-blocks" ] );
    ExpectEffect( entity[ "perceptions/cones" ], sword::test::MakeModel()
        [ sword::test::MakeModel( "origin/x", 0 )
                                ( "origin/y", 1 )
                                ( "height", 0 )
                                ( "sensor", "sensor-type" )
                                ( "sectors", sword::test::MakeModel()[ sword::test::MakeModel( "direction/x", 1 )( "direction/y", 0 ) ] ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
