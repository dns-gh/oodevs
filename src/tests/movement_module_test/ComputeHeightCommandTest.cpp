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

BOOST_FIXTURE_TEST_CASE( compute_height_command_updates_entity_height_every_step, sword::movement::ModuleFixture )
{
    MOCK_RESET( ComputeHeight );
    const int identifier = 77;
    StartCommand( "compute height", core::MakeModel( "identifier", identifier ) );
    const core::Model& target = model[ "entities" ][ identifier ][ "movement" ];
    ExpectEffect( target, sword::test::MakeModel( "height", 42 ) );
    MOCK_EXPECT( ComputeHeight ).once().returns( 42 );
    ExecuteCommands();
    mock::verify();
    ExpectEffect( target, sword::test::MakeModel( "height", 42 ) );
    MOCK_EXPECT( ComputeHeight ).once().returns( 42 );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( paused_compute_height_command_does_nothing, sword::movement::ModuleFixture )
{
    commands.Pause( StartCommand( "compute height", core::MakeModel( "identifier", 77 ) ) );
    ExecuteCommands();
}
