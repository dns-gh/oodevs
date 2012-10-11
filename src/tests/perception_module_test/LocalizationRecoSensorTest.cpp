// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "PerceptionCommandFixture.h"

using namespace sword::perception;

BOOST_FIXTURE_TEST_CASE( localization_reco_sensor_recognized_all_agents_in_location_depending_growing_speed, PerceptionCommandFixture )
{
    const std::size_t perceptionId = 42;
    entity[ "perceptions/sensor/activated" ] = false;
    core::Model& perception = entity[ "perceptions/reco" ][ perceptionId ];
    perception = core::MakeModel( "localization", 43 )
                                ( "perception-id", perceptionId )
                                ( "growth-speed", 2 )
                                ( "has-growth-speed", true )
                                ( "radius", 0 )
                                ( "max-radius-reached", false );
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    model[ "entities" ][ target ] = core::MakeModel( "data", 43 )
                                                   ( "movement/position/x", 5 )
                                                   ( "movement/position/y", 5 );
    MOCK_EXPECT( GetLocalizationRadius ).once().returns( 100 );
    MOCK_EXPECT( IsPointInsideLocalisation ).once().returns( true );
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( CanBeSeen ).once().returns( true );
    ExpectEffect( perception[ "radius" ], sword::test::MakeModel( 2 ) );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target", mock::any )
                                                            ( "level", 2 ) // recognized
                                                            ( "recorded", false ) ]
                                    [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
