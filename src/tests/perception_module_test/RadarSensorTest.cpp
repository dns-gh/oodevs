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

namespace
{
    struct RadarFixture : public PerceptionCommandFixture
    {
        RadarFixture()
        {
            core::Model& other = model[ "entities/13337" ];
            other = core::MakeModel( "pion", core::MakeUserData( reinterpret_cast< MIL_Agent_ABC* >( 43 ) ) )
                                   ( "movement/position/x", 5 )
                                   ( "movement/position/y", 5 )
                                   ( "movement/height", 5 )
                                   ( "is-pc", false )
                                   ( "identifier", 13337 );
        }
        void Tick( unsigned int identificationLevel )
        {
            if( model[ "tick" ] > 0 )
            {
                ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
                ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
                ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
                ExpectEffect( entity[ "perceptions/main-perception-direction" ] );
                ExpectEffect( entity[ "perceptions/radars/acquisitions" ] );
            }
            MOCK_EXPECT( GetConsumptionMode ).once().returns( 0u );
            MOCK_EXPECT( CanBeSeen ).once().returns( true );
            ExpectNotifications( "agents", sword::test::MakeModel()
                                            [ sword::test::MakeModel( "target", 43 )
                                                                    ( "level", identificationLevel )
                                                                    ( "recorded", false ) ]
                                            [ sword::test::MakeModel( mock::any ) ] );
            commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
            commands.Execute();
            model[ "tick" ] = model[ "tick" ] + 1;
        }
    };
}

BOOST_FIXTURE_TEST_CASE( radar_sensor_acquire_targets_and_waits_ticks_before_notifying_detection_recognition_or_identification, RadarFixture )
{
    core::Model& other = model[ "entities/13337" ];
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/radars/radar/activated" ] = true;
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, core::Convert( &other ), _5 ) );
    ExpectEffect( entity[ "perceptions/radars/acquisitions" ],
                        sword::test::MakeModel( "my-radar/13337", sword::test::MakeModel( "first-step", 0u )
                                                                                        ( "identifier", 13337 ) ) );
    Tick( 0u ); // not seen
    entity[ "perceptions/radars/acquisitions/my-radar/other/first-step" ] = 0u;
    entity[ "perceptions/radars/acquisitions/my-radar/other/identifier" ] = 13337;
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, core::Convert( &other ), _5 ) );
    Tick( 1u ); // detected
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, core::Convert( &other ), _5 ) );
    Tick( 2u ); // recognized
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, core::Convert( &other ), _5 ) );
    Tick( 3u ); // identified
}

BOOST_FIXTURE_TEST_CASE( localized_radar_sensor_acquire_targets_and_waits_ticks_before_notifying_detection_recognition_or_identification, RadarFixture )
{
    core::Model& other = model[ "entities/13337" ];
    const TER_Localisation* localization = reinterpret_cast< const TER_Localisation* >( 0xCAFE );
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/localized-radars/radar/421/localization" ].SetUserData( localization );
    entity[ "perceptions/localized-radars/radar/421/perception-id" ] = 421u;
    ExpectEffect( entity[ "perceptions/radars/acquisitions" ],
                        sword::test::MakeModel( "my-radar/13337", sword::test::MakeModel( "first-step", 0u )
                                                                                        ( "identifier", 13337 ) ) );
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, core::Convert( &other ), _4 ) );
    Tick( 0u ); // not seen
    entity[ "perceptions/radars/acquisitions/my-radar/other/first-step" ] = 0u;
    entity[ "perceptions/radars/acquisitions/my-radar/other/identifier" ] = 13337;
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, core::Convert( &other ), _4 ) );
    Tick( 1u ); // detected
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, core::Convert( &other ), _4 ) );
    Tick( 2u ); // recognized
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().calls( boost::bind( boost::apply< void >(), _3, core::Convert( &other ), _4 ) );
    Tick( 3u ); // identified
}