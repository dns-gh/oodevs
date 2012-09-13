// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "ModuleFixture.h"

BOOST_FIXTURE_TEST_CASE( vision_command_changes_mode_to_direction, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 3;
    core::Model& vision = model[ "entities" ][ identifier ][ "perceptions/vision" ];
    vision[ "mode" ] = "normal";
    ExpectEffect( vision, sword::test::MakeModel( "mode", "direction" )
                                                ( "location/x", 0.1 )
                                                ( "location/y", 0.2 ) );
    StartCommand( "vision",
        core::MakeModel( "identifier", identifier )
                       ( "mode", "direction" )
                       ( "location/x", 0.1 )
                       ( "location/y", 0.2 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( vision_command_changes_mode_to_location, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 3;
    core::Model& vision = model[ "entities" ][ identifier ][ "perceptions/vision" ];
    vision[ "mode" ] = "normal";
    ExpectEffect( vision, sword::test::MakeModel( "mode", "location" )
                                                ( "location/x", 0.1 )
                                                ( "location/y", 0.2 ) );
    StartCommand( "vision",
        core::MakeModel( "identifier", identifier )
                       ( "mode", "location" )
                       ( "location/x", 0.1 )
                       ( "location/y", 0.2 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( vision_command_changes_mode_to_normal_and_uses_current_movement_direction, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 3;
    core::Model& entity = model[ "entities" ][ identifier ];
    entity[ "movement/direction/x" ] = 0.1;
    entity[ "movement/direction/y" ] = 0.2;
    core::Model& vision = entity[ "perceptions/vision" ];
    vision[ "mode" ] = "location";
    ExpectEffect( vision, sword::test::MakeModel( "mode", "normal" )
                                                ( "location/x", 0.1 )
                                                ( "location/y", 0.2 ) );
    StartCommand( "vision",
        core::MakeModel( "identifier", identifier )
                       ( "mode", "normal" ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( unknown_vision_mode_sends_error_log, sword::perception::ModuleFixture )
{
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, mock::any );
    StartCommand( "vision",
        core::MakeModel( "identifier", 2 )
                       ( "mode", "unknown" ) );
    ExecuteCommands();
}
