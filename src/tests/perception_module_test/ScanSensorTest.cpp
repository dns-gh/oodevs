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
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/scan/activated" ] = true;
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    model[ "entities" ][ target ][ "pion" ] = 43;
    model[ "entities" ][ target ][ "movement/position/x" ] = 5;
    model[ "entities" ][ target ][ "movement/position/y" ] = 5;
    model[ "entities" ][ target ][ "identifier" ] = 1337u;
    const SWORD_Model* perceiver = core::Convert( &entity );
    const double scanWidth = 100;
    const double scanLength = 50;
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::any, scanLength, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( BelongsToKnowledgeGroup ).with( perceiver, other ).returns( false );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    entity[ "perceptions/drill-blow/width" ] = scanWidth;
    entity[ "perceptions/drill-blow/length" ] = scanLength;
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( "target", 43 )
                                                               ( "level", 2 ) // recognized
                                                               ( "recorded", false ) ]
                                       [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
