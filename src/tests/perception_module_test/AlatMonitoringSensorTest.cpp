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

BOOST_FIXTURE_TEST_CASE( alat_monitoring_sensor_recognized_all_agents_in_location_depending_vision_objects_in_surface, PerceptionCommandFixture )
{
    const unsigned char eVisionEmpty = 0;
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/alat/monitoring" ][ 42 ] = core::MakeModel( "localization", 0 )
                                                                   ( "empty-detection-time-step", 0 )
                                                                   ( "forest-detection-time-step", 1 )
                                                                   ( "urban-detection-time-step", 2 );
    core::Model& other = model[ "entities" ][ target ];
    other = core::MakeModel( "data", 43 )
                           ( "movement/position/x", 5 )
                           ( "movement/position/y", 5 );
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, core::Convert( &other ), _4 ) );
    MOCK_EXPECT( IsPointInsideLocalisation ).once().returns( true );
    MOCK_EXPECT( CanBeSeen ).once().with( core::Convert( &entity ), core::Convert( &other ) ).returns( true );
    MOCK_EXPECT( GetVisionObject ).once().returns( eVisionEmpty );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target/data", 43 )
                                                            ( "level", 2 ) // recognized
                                                            ( "recorded", false ) ]
                                    [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
