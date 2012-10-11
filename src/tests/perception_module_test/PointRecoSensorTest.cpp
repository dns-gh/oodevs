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

BOOST_FIXTURE_TEST_CASE( perception_reco_point_sensor_recognizes_all_agents_in_growing_circle_and_growth_speed, PerceptionCommandFixture )
{
    const double growthSpeed = 2;
    entity[ "perceptions/sensor/activated" ] = false;
    const std::size_t perceptionId = 42;
    core::Model& perception = entity[ "perceptions/recognition-point" ][ perceptionId ];
    perception = core::MakeModel( "perception-id", perceptionId )
                                ( "growth-speed", growthSpeed )
                                ( "center/x", 10 )
                                ( "center/y", 20 )
                                ( "max-radius", 10 )
                                ( "radius", 0 )
                                ( "max-radius-reached", false );
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    const SWORD_Model* perceiver = core::Convert( &entity );
    model[ "entities" ][ target ] = core::MakeModel( "data", 43 )
                                                   ( "identifier", target );
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::any, growthSpeed, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    ExpectEffect( perception[ "radius" ], sword::test::MakeModel( growthSpeed ) );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target/data", 43 )
                                                            ( "level", 2 ) // recognized
                                                            ( "recorded", false ) ]
                                    [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( perception_reco_point_sensor_identifies_all_objects_in_growing_circle_and_growth_speed, PerceptionCommandFixture )
{
    const double growthSpeed = 2;
    const SWORD_Model* object = core::Convert( &model[ "objects/some-object" ] );
    model[ "objects/some-object" ] = core::MakeModel( "data", 666 )
                                                    ( "can-be-perceived", true );
    entity[ "perceptions/sensor/activated" ] = false;
    const std::size_t perceptionId = 42;
    core::Model& perception = entity[ "perceptions/recognition-point" ][ perceptionId ];
    perception = core::MakeModel( "perception-id", perceptionId )
                                ( "growth-speed", growthSpeed )
                                ( "center", core::MakeModel( "x", 10 )
                                                           ( "y", 20 ) )
                                ( "radius", 0 )
                                ( "max-radius", 10 )
                                ( "max-radius-reached", false );
    MOCK_RESET( GetObjectListWithinCircle );
    MOCK_EXPECT( GetObjectListWithinCircle ).once();
    MOCK_EXPECT( GetObjectListWithinCircle ).once().with( mock::any, mock::any, growthSpeed, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _4, object, _5 ) );
    ExpectEffect( perception[ "radius" ], sword::test::MakeModel( growthSpeed ) );
    ExpectNotifications( "objects", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target/data", 666 )
                                                            ( "target/can-be-perceived", true )
                                                            ( "level", 3 ) // identified
                                                            ( "recorded", false ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
