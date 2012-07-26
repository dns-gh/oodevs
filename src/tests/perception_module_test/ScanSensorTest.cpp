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

BOOST_FIXTURE_TEST_CASE( agents_in_list_are_recognized_with_scan_sensor, PerceptionCommandFixture )
{
    const SWORD_Model* other = core::Convert( &model[ "entities/other" ] );
    model[ "entities/other/pion" ].SetUserData< MIL_Agent_ABC* >( reinterpret_cast< MIL_Agent_ABC* >( 43 ) );
    model[ "entities/other/movement/position/x" ] = 5;
    model[ "entities/other/movement/position/y" ] = 5;
    model[ "entities/other/identifier" ] = 1337u;
    const SWORD_Model* perceiver = core::Convert( &entity );
    const double scanWidth = 100;
    const double scanLength = 50;
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::any, scanLength, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( BelongsToKnowledgeGroup ).with( perceiver, other ).returns( false );
    MOCK_EXPECT( IsAgentIdentified ).once().with( perceiver, other ).returns( false );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/scan/activated" ] = true;
    entity[ "perceptions/drill-blow/width" ] = scanWidth;
    entity[ "perceptions/drill-blow/length" ] = scanLength;
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( "target", 43 )
                                                               ( "level", 2 ) // recognized
                                                               ( "recorded", false ) ]
                                       [ sword::test::MakeModel( mock::any ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}
