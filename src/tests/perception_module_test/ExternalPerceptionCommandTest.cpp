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
    model[ "entities" ][ 3 ][ "data" ] = "data1";
    model[ "entities" ][ 4 ][ "data" ] = "data2";
    ExpectEvent( "external perception", sword::test::MakeModel( "entity/data", "data1" )
                                                              ( "level", 2 )
                                                              ( "target/data", "data2" ) );
    StartCommand( "external perception",
        core::MakeModel( "perceiver", 3 )
                       ( "level", 2 )
                       ( "target", 4 ) );
    ExecuteCommands();
}
