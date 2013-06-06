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

BOOST_FIXTURE_TEST_CASE( magic_move_command_posts_effect_on_position_and_resets_speed_and_path, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", -1 )
                                                                     ( "position/y", -2 )
                                                                     ( "speed", 30 );
    PostCommand( "magic move", core::MakeModel( "identifier", identifier )
                                              ( "position/x", 1 )
                                              ( "position/y", 2 ) );
    ExpectEffect( model[ "entities" ][ identifier ][ "movement" ],
                  sword::test::MakeModel( "position/x", 1 )
                                        ( "position/y", 2 )
                                        ( "speed", 0 )
                                        ( "path/points", sword::test::MakeModel() ) );
    ExecuteCommands();
}
