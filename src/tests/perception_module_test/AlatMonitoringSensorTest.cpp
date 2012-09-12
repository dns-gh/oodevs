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
    entity[ "perceptions/alat/monitoring/0/localization" ];
    entity[ "perceptions/alat/monitoring/0/empty-detection-time-step" ] = 0;
    entity[ "perceptions/alat/monitoring/0/forest-detection-time-step" ] = 1;
    entity[ "perceptions/alat/monitoring/0/urban-detection-time-step" ] = 2;
    const SWORD_Model* other = core::Convert( &model[ "entities/other" ] );
    model[ "entities/other/pion" ] = 43;
    model[ "entities/other/movement/position/x" ] = 5;
    model[ "entities/other/movement/position/y" ] = 5;
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, other, _4 ) );
    MOCK_EXPECT( IsPointInsideLocalisation ).once().returns( true );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( GetVisionObject ).once().returns( eVisionEmpty );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target", 43 )
                                                            ( "level", 2 ) // recognized
                                                            ( "recorded", false ) ]
                                    [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
