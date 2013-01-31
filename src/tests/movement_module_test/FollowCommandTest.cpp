// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "movement_module_test_pch.h"
#include "ModuleFixture.h"

BOOST_FIXTURE_TEST_CASE( follow_command_posts_effect_with_followed_position_and_direction, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    const std::size_t followed = 43;
    model[ "entities" ][ followed ][ "movement" ] = core::MakeModel( "position/x", 1 )
                                                                   ( "position/y", 2 )
                                                                   ( "direction/x", 1 )
                                                                   ( "direction/y", 1 )
                                                                   ( "speed", 20 )
                                                                   ( "height", 10 );
    model[ "entities" ][ identifier ][ "movement/intention" ] = false;
    ExpectEffect( model[ "entities" ][ identifier ][ "movement/intention" ], sword::test::MakeModel( true ) );
    PostCommand( "follow", core::MakeModel( "identifier", identifier )
                                          ( "followed", followed ) );
    ExpectEffect( model[ "entities" ][ identifier ][ "movement" ],
                  sword::test::MakeModel( "position/x", 1 )
                                        ( "position/y", 2 )
                                        ( "direction/x", 1 )
                                        ( "direction/y", 1 )
                                        ( "speed", 20 )
                                        ( "height", 10 ) );
    ExpectEffect( model[ "entities" ][ identifier ][ "movement/intention" ], sword::test::MakeModel( false ) );
    ExecuteCommands();
}
