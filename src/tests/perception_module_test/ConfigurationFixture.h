// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CONFIGURATION_FIXTURE_H
#define CONFIGURATION_FIXTURE_H

#include "ModuleFixture.h"
#include <boost/bind/apply.hpp>

namespace sword
{
namespace perception
{
    inline double ConvertSeconds( double seconds )
    {
        return seconds / 10.;
    }
    inline bool Assign( size_t* identifier )
    {
        *identifier = 0;
        return true;
    }
    struct ConfigurationFixture : public ModuleFixture
    {
        ConfigurationFixture()
        {
            const std::string xml( "<sensors>"
                "  <alat-monitoring-times>"
                "    <alat-monitoring-time terrain='Vide' time='1s'/>"
                "    <alat-monitoring-time terrain='Foret' time='5s'/>"
                "    <alat-monitoring-time terrain='Urbain' time='20s'/>"
                "  </alat-monitoring-times>"
                "  <cobra-radar action-range='100000'/>"
                "  <radars>"
                "    <radar action-range='60000' name='my-radar' type='radar'>"
                "      <detectable-activities>"
                "        <detectable-activity type='activity' value='true'/>"
                "      </detectable-activities>"
                "      <acquisition-times>"
                "        <acquisition-time base-time='10s' level='detection'/>"
                "        <acquisition-time base-time='20s' level='recognition'/>"
                "        <acquisition-time base-time='30s' level='identification'/>"
                "      </acquisition-times>"
                "    </radar>"
                "  </radars>"
                "  <sensors>"
                "    <sensor detection-delay='0h' name='sensor-type'>"
                "      <unit-detection angle='120' firer-detection-distance='0' scanning='true'>"
                "        <base-distances close-range='100'>"
                "          <base-distance distance='300' level='identification'/>"
                "          <base-distance distance='1500' level='recognition'/>"
                "          <base-distance distance='4000' level='detection'/>"
                "        </base-distances>"
                "        <distance-modifiers>"
                "          <population-modifier density='0.5' modifier='0.1'/>"
                "          <size-modifiers>"
                "            <distance-modifier type='volume' value='1'/>"
                "          </size-modifiers>"
                "          <precipitation-modifiers>"
                "            <distance-modifier type='precipitation' value='1'/>"
                "          </precipitation-modifiers>"
                "          <visibility-modifiers>"
                "            <distance-modifier type='lighting' value='1'/>"
                "          </visibility-modifiers>"
                "          <source-posture-modifiers>"
                "            <distance-modifier type='posture' value='1'/>"
                "          </source-posture-modifiers>"
                "          <target-posture-modifiers>"
                "            <distance-modifier type='posture' value='1'/>"
                "          </target-posture-modifiers>"
                "          <terrain-modifiers/>"
                "          <urbanBlock-material-modifiers>"
                "            <distance-modifier type='urban' value='1'/>"
                "          </urbanBlock-material-modifiers>"
                "        </distance-modifiers>"
                "      </unit-detection>"
                "      <object-detection>"
                "        <object detection-distance='300' type='object'>"
                "          <population-modifier density='2' modifier='0.5'/>"
                "          <source-posture-modifiers>"
                "            <distance-modifier type='posture' value='1'/>"
                "          </source-posture-modifiers>"
                "        </object>"
                "      </object-detection>"
                "    </sensor>"
                "  </sensors>"
                "</sensors>" );
            MOCK_EXPECT( GetConsumptionTypeSize ).returns( 1 );
            MOCK_EXPECT( FindConsumptionType ).with( std::string( "activity" ), mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _2, 0, _3 ) );
            MOCK_EXPECT( ConvertSecondsToSim ).calls( boost::bind( &ConvertSeconds, _1 ) );
            MOCK_EXPECT( GetVolumeSize ).returns( 1 );
            MOCK_EXPECT( GetVolumeIdentifier ).with( std::string( "volume" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( GetPrecipitationSize ).returns( 1 );
            MOCK_EXPECT( GetPrecipitationIdentifier ).with( std::string( "precipitation" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( GetLightingSize ).returns( 1 );
            MOCK_EXPECT( GetLightingIdentifier ).with( std::string( "lighting" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( GetPostureSize ).returns( 1 );
            MOCK_EXPECT( GetPostureIdentifier ).with( std::string( "posture" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( PostureCanModifyDetection ).returns( true );
            MOCK_EXPECT( GetMaterialTypeSize ).returns( 1 );
            MOCK_EXPECT( IsMaterialType ).with( std::string( "urban" ) ).returns( true );
            MOCK_EXPECT( FindObjectType ).with( std::string( "object" ) ).returns( 0 );
            InitializePerceptionTypes( xml.c_str() );
        }
    };
}
}

#endif // CONFIGURATION_FIXTURE_H
