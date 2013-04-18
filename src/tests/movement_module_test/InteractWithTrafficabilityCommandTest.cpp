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

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_posts_event_with_agent_list_to_interact_with, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 10 )
                                                                     ( "position/y", 11 )
                                                                     ( "can-be-traffic-impacted", true );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    core::Model& interactionAgent = model[ "entities" ][ 1337u ];
    interactionAgent[ "data" ] = "data 1337";
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::equal( MT_Vector2D( 10, 11 ) ), 100.f, mock::any, mock::any )
                                           .calls( boost::bind( boost::apply< void >(), _4, core::Convert( &interactionAgent ), _5 ) );
    ExpectEvent( "trafficability interaction", sword::test::MakeModel( "entity", identifier )
                                                                     ( "interactions", sword::test::MakeModel()[ sword::test::MakeModel( "data", "data 1337" ) ] ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_does_not_post_event_with_empty_agent_list, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 10 )
                                                                     ( "position/y", 11 )
                                                                     ( "can-be-traffic-impacted", true );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( interact_with_trafficability_command_return_if_agent_not_traffic_impactable, sword::movement::ModuleFixture )
{
    const std::size_t identifier = 42;
    model[ "entities" ][ identifier ][ "movement" ] = core::MakeModel( "position/x", 10 )
                                                                     ( "position/y", 11 )
                                                                     ( "can-be-traffic-impacted", false );
    model[ "entities" ][ identifier ][ "perceptions/max-agent-perception-distance" ] = 100.f;
    PostCommand( "interact with trafficability", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
