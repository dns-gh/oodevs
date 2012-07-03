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

BOOST_FIXTURE_TEST_CASE( orientate_command_changes_the_direction, sword::movement::ModuleFixture )
{
    const int identifier = 77;
    commands.Start( "orientate",
        core::MakeModel( "identifier", identifier )
            ( "direction/x", 12 )
            ( "direction/y", 42 ) );
    ExpectEffect( model[ "entities" ][ identifier ][ "movement" ], sword::test::MakeModel( "direction/x", 12 )
                                                                                         ( "direction/y", 42 ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( paused_orientate_command_does_nothing, sword::movement::ModuleFixture )
{
    commands.Pause(
        commands.Start( "orientate",
            core::MakeModel( "identifier", 77 )
                ( "direction/x", 12 )
                ( "direction/y", 42 ) ) );
    commands.Execute();
}
