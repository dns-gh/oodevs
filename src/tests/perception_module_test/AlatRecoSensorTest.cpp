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

BOOST_FIXTURE_TEST_CASE( alat_reco_sensor_recognized_all_agents_in_location_depending_max_perception_distance, PerceptionCommandFixture )
{
    const unsigned char eVisionEmpty = 0;
    const std::size_t uniqueReco = 0;
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/alat/reco" ][ uniqueReco ][ "localization" ];
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    model[ "entities" ][ target ][ "data" ] = 43;
    model[ "entities" ][ target ][ "movement/position/x" ] = 5;
    model[ "entities" ][ target ][ "movement/position/y" ] = 5;
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, other, _4 ) );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( GetVisionObject ).once().returns( eVisionEmpty );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target", mock::any )
                                                            ( "level", 2 ) // recognized
                                                            ( "recorded", false ) ]
                                    [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
