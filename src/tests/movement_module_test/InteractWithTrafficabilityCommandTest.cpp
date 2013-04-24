// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "movement_module_test_pch.h"
#include "ModuleFixture.h"
#include <boost/bind/apply.hpp>

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_posts_effect_with_agent_list_to_interact_with, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 10 )
                                                                     ( "position/y", 11 )
                                                                     ( "direction/x", 1 )
                                                                     ( "direction/y", 1 )
                                                                     ( "speed-modifier", 0.5 )
                                                                     ( "can-be-traffic-impacted", true )
                                                                     ( "foot-print-radius", 10 )
                                                                     ( "traffic-modifier", 1. );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    core::Model& interactionAgent = model[ "entities" ][ 1337u ];
    interactionAgent[ "identifier" ] = 1337;
    interactionAgent[ "data" ] = "data 1337";
    interactionAgent[ "movement" ][ "can-interact-with-traffic" ] = true;
    interactionAgent[ "movement" ][ "position/x" ] = 12;
    interactionAgent[ "movement" ][ "position/y" ] = 13;
    interactionAgent[ "movement" ][ "foot-print-radius" ] = 10;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::equal( MT_Vector2D( 10, 11 ) ), 100.f, mock::any, mock::any )
                                           .calls( boost::bind( boost::apply< void >(), _4, core::Convert( &interactionAgent ), _5 ) );
    ExpectEffect( model[ "entities" ][ identifier ][ "movement" ], sword::test::MakeModel( "traffic-modifier", 0.5 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_does_not_post_effect_with_agent_behind_entity, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 0 )
                                                                     ( "position/y", 1 )
                                                                     ( "direction/x", 0 )
                                                                     ( "direction/y", 1 )
                                                                     ( "speed-modifier", 0.5 )
                                                                     ( "can-be-traffic-impacted", true )
                                                                     ( "foot-print-radius", 10 )
                                                                     ( "traffic-modifier", 1. );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    core::Model& interactionAgent = model[ "entities" ][ 1337u ];
    interactionAgent[ "identifier" ] = 1337;
    interactionAgent[ "data" ] = "data 1337";
    interactionAgent[ "movement" ][ "can-interact-with-traffic" ] = true;
    interactionAgent[ "movement" ][ "position/x" ] = 0;
    interactionAgent[ "movement" ][ "position/y" ] = 0;
    interactionAgent[ "movement" ][ "foot-print-radius" ] = 10;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::equal( MT_Vector2D( 0, 1 ) ), 100.f, mock::any, mock::any )
        .calls( boost::bind( boost::apply< void >(), _4, core::Convert( &interactionAgent ), _5 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_does_not_post_effect_with_agent_outside_footprint_distance, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 0 )
                                                                     ( "position/y", 1 )
                                                                     ( "direction/x", 0 )
                                                                     ( "direction/y", 1 )
                                                                     ( "speed-modifier", 0.5 )
                                                                     ( "can-be-traffic-impacted", true )
                                                                     ( "foot-print-radius", 2 )
                                                                     ( "traffic-modifier", 1. );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    core::Model& interactionAgent = model[ "entities" ][ 1337u ];
    interactionAgent[ "identifier" ] = 1337;
    interactionAgent[ "data" ] = "data 1337";
    interactionAgent[ "movement" ][ "can-interact-with-traffic" ] = true;
    interactionAgent[ "movement" ][ "position/x" ] = 0;
    interactionAgent[ "movement" ][ "position/y" ] = 5;
    interactionAgent[ "movement" ][ "foot-print-radius" ] = 2;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::equal( MT_Vector2D( 0, 1 ) ), 100.f, mock::any, mock::any )
        .calls( boost::bind( boost::apply< void >(), _4, core::Convert( &interactionAgent ), _5 ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_does_not_post_effect_with_empty_agent_list, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 10 )
                                                                     ( "position/y", 11 )
                                                                     ( "can-be-traffic-impacted", true )
                                                                     ( "traffic-modifier", 1. );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_does_not_post_effect_if_entity_not_traffic_impactable, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 10 )
                                                                     ( "position/y", 11 )
                                                                     ( "can-be-traffic-impacted", false )
                                                                     ( "traffic-modifier", 1. );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
