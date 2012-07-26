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
                                                                                         ( "target", reinterpret_cast< size_t >( agent ) )
                                                                                         ( "recorded", false ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_its_transporter, PerceptionCommandFixture )
{
    const size_t transporterId = 1342;
    const SWORD_Model* transporter = core::Convert( &model[ "entities" ][ transporterId ] );
    const MIL_Agent_ABC* transporterPion = reinterpret_cast< MIL_Agent_ABC* >( 1818 );
    model[ "entities" ][ transporterId ][ "pion" ].SetUserData( transporterPion );
    MOCK_RESET( GetTransporter );
    MOCK_EXPECT( GetTransporter ).returns( transporter );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( mock::any ) ]
                                       [ sword::test::MakeModel( "level", 3 )
                                                               ( "target", reinterpret_cast< size_t >( transporterPion ) )
                                                               ( "recorded", false ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_urban_objects_in_list, PerceptionCommandFixture )
{
    UrbanObjectWrapper* urbanObject = reinterpret_cast< UrbanObjectWrapper* >( 42 );
    MOCK_RESET( GetUrbanObjectListWithinCircle );
    MOCK_EXPECT( GetUrbanObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, urbanObject, _4 ) );
    MOCK_EXPECT( GetUrbanObjectOccupation ).once().with( urbanObject ).returns( 1 );
    ExpectNotifications( "urban-blocks", sword::test::MakeModel()
                                             [ sword::test::MakeModel( "level", 3 )
                                                                     ( "target", reinterpret_cast< size_t >( urbanObject ) ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( perception_command_updates_next_peripherical_vision_step, PerceptionCommandFixture )
{
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
    mock::verify();
    model[ "step" ] = 1;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ], sword::test::MakeModel( "next-step", 13 )
                                                                                     ( "activated", true ) );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ] );
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( perception_command_with_direction_vision_uses_movement_direction_as_main_perception_direction, PerceptionCommandFixture )
{
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
    mock::verify();
    model[ "step" ] = 1;
    entity[ "perceptions/vision/mode" ] = "direction";
    entity[ "perceptions/vision/location/x" ] = 42.;
    entity[ "perceptions/vision/location/y" ] = 43.;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", 42. )
                                                                                           ( "y", 43. ) );
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( perception_command_with_location_vision_mode_computes_location_direction_as_main_perception_direction, PerceptionCommandFixture )
{
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
    mock::verify();
    model[ "step" ] = 1;
    entity[ "perceptions/vision/mode" ] = "location";
    entity[ "perceptions/vision/location/x" ] = 0;
    entity[ "perceptions/vision/location/y" ] = 0;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", 0 )
                                                                                           ( "y", -1 ) );
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}
