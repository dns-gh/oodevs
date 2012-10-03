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

BOOST_FIXTURE_TEST_CASE( external_perception_command_produces_external_perception_event, sword::perception::ModuleFixture )
{
    ExpectEvent( "external perception", sword::test::MakeModel( "identifier", 3 )
                                                              ( "level", 2 )
                                                              ( "target/data", 0x123 ) );
    StartCommand( "external perception",
        core::MakeModel( "identifier", 3 )
                       ( "level", 2 )
                       ( "target/data", 0x123 ) );
    ExecuteCommands();
}
