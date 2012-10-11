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

BOOST_FIXTURE_TEST_CASE( flying_shell_sensor_posts_event_when_flying_shell_is_inside_zone, PerceptionCommandFixture )
{
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/flying-shell/zones" ][ 42u ][ "localization" ];
    model[ "flying-shells" ].AddElement() = core::MakeModel( "identifier", 33u )
                                                           ( "data", "data" );
    MOCK_EXPECT( CanFlyingShellBePerceived ).once().returns( true );
    ExpectEffect( entity[ "perceptions/flying-shell/previous" ], sword::test::MakeModel()[ sword::test::MakeModel( 33u ) ] );
    ExpectEvent( "flying shell detection", sword::test::MakeModel( "entity", mock::any )
                                                                 ( "flying-shell", mock::any ) );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExpectNotifications();
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( flying_shell_sensor_posts_event_only_once, PerceptionCommandFixture )
{
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/flying-shell/zones" ][ 42u ][ "localization" ];
    model[ "flying-shells" ].AddElement() = core::MakeModel( "identifier", 33u )
                                                           ( "data", "data" );
    entity[ "perceptions/flying-shell/previous" ].AddElement()[ "identifier" ] = 33u;
    MOCK_EXPECT( CanFlyingShellBePerceived ).once().returns( true );
    ExpectEffect( entity[ "perceptions/flying-shell/previous" ], sword::test::MakeModel()[ sword::test::MakeModel( 33u ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExpectNotifications();
    ExecuteCommands();
}
