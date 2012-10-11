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
#include <boost/bind/apply.hpp>

BOOST_FIXTURE_TEST_CASE( identify_all_agents_in_zone_command_notifies_perceptions, sword::perception::ModuleFixture )
{
    const unsigned int identifier = 3;
    const unsigned int target = 13;
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    model[ "entities" ][ target ][ "data" ] = 43;
    core::Model& zone = model[ "entities" ][ identifier ][ "perceptions/notifications/agents-in-zone" ];
    model[ "entities" ][ identifier ][ "perceptions/record-mode/activated" ] = false;
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, other, _4 ) );
    ExpectEffect( zone, sword::test::MakeModel()
                          [ sword::test::MakeModel( "level", 3 )
                                                  ( "target/data", 43 )
                                                  ( "recorded", false ) ] );
    StartCommand( "identify all agents in zone",
        core::MakeModel( "identifier", identifier )
                       ( "localization", core::MakeModel() ) );
    ExecuteCommands();
}
