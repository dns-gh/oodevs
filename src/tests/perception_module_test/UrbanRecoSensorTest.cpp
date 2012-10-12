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

BOOST_FIXTURE_TEST_CASE( perception_reco_urban_sensor_recognizes_all_agents_in_urban_block_localization, PerceptionCommandFixture )
{
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/urban" ][ 42 ] [ "localization/block" ];
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    const SWORD_Model* perceiver = core::Convert( &entity );
    model[ "entities" ][ target ] = core::MakeModel( "data", 43 )
                                                   ( "identifier", target );
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, other, _4 ) );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( CanUrbanBlockBeSeen ).once().returns( true );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                    [ sword::test::MakeModel( "target/data", 43 )
                                                            ( "level", 2 ) // recognized
                                                            ( "recorded", false ) ]
                                    [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
