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

BOOST_FIXTURE_TEST_CASE( perception_reco_object_sensor_identifies_all_objects_in_location_depending_circle_and_growth_speed, PerceptionCommandFixture )
{
    const double growthSpeed = 2;
    const std::size_t perceptionId = 42;
    boost::shared_ptr< TER_Localisation > localization;
    MIL_Object_ABC* object = reinterpret_cast< MIL_Object_ABC* >( 666 );
    entity[ "perceptions/sensor/activated" ] = false;
    core::Model& perception = entity[ "perceptions/object-detection" ][ perceptionId ];
    perception = core::MakeModel( "localization", core::MakeUserData( localization ) )
                                ( "perception-id", perceptionId )
                                ( "growth-speed", growthSpeed )
                                ( "center", core::MakeModel( "x", 10 )
                                                           ( "y", 20 ) )
                                ( "radius", 0 )
                                ( "max-radius-reached", false );
    MOCK_EXPECT( IsLocalizationInsideCircle ).once().returns( false );
    MOCK_RESET( GetObjectListWithinCircle );
    MOCK_EXPECT( GetObjectListWithinCircle ).once();
    MOCK_EXPECT( GetObjectListWithinCircle ).once().with( mock::any, growthSpeed, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _3, object, _4 ) );
    MOCK_EXPECT( IsObjectIntersectingLocalization ).once().with( mock::any, object ).returns( true );
    MOCK_EXPECT( CanObjectBePerceived ).once().with( object ).returns( true );
    ExpectEffect( perception[ "radius" ], sword::test::MakeModel( growthSpeed ) );
    ExpectEvent( "perception callback", sword::test::MakeModel( "entity", identifier )
                                                              ( "perception", perceptionId ) );
    ExpectNotifications( "objects", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target", 666 )
                                                            ( "level", 3 ) // identified
                                                            ( "recorded", false ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}
