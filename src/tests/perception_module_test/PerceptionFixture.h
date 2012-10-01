// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PERCEPTION_FIXTURE_H
#define PERCEPTION_FIXTURE_H

#include "ConfigurationFixture.h"
#include "MT_Tools/MT_Vector2D.h"

namespace sword
{
namespace perception
{
    struct PerceptionFixture : public ConfigurationFixture
    {
        PerceptionFixture()
            : identifier           ( 42 )
            , maxPerceptionDistance( 100 )
            , theoreticalDistance  ( 120 )
            , position             ( 0, 1 )
            , direction            ( 1, 0 )
            , entity               ( model[ "entities" ][ identifier ] )
        {
            model[ "tick" ] = 0;
            entity[ "identifier" ] = identifier;
            entity[ "is-dead" ] = false;
            entity[ "is-underground" ] = false;
            entity[ "is-transported" ] = false;
            entity[ "is-prisoner" ] = false;
            entity[ "pion" ] = 1337;
            entity[ "perceptions/max-agent-perception-distance" ] = maxPerceptionDistance;
            entity[ "perceptions/max-theoretical-agent-perception-distance" ] = theoreticalDistance;
            entity[ "perceptions/peripherical-vision/next-tick" ] = 1;
            entity[ "perceptions/peripherical-vision/activated" ] = false;
            entity[ "perceptions/vision/mode" ] = "normal";
            entity[ "perceptions/cones" ];
            entity[ "perceptions/notifications/agents" ];
            entity[ "perceptions/notifications/objects" ];
            entity[ "perceptions/notifications/population-flows" ];
            entity[ "perceptions/notifications/population-concentrations" ];
            entity[ "perceptions/notifications/urban-blocks" ];
            entity[ "movement/position/x" ] = position.rX_;
            entity[ "movement/position/y" ] = position.rY_;
            entity[ "movement/direction/x" ] = direction.rX_;
            entity[ "movement/direction/y" ] = direction.rY_;
            entity[ "movement/height" ] = 0;
            entity[ "perceptions/sensor/activated" ] = true;
            entity[ "perceptions/scan/activated" ] = false;
            entity[ "perceptions/alat/reco" ];
            entity[ "perceptions/alat/monitoring" ];
            entity[ "perceptions/object-detection" ];
            entity[ "perceptions/urban" ];
            entity[ "perceptions/reco" ];
            entity[ "perceptions/flying-shell/zones" ];
            entity[ "perceptions/flying-shell/previous" ];
            entity[ "perceptions/recognition-point" ];
            entity[ "perceptions/radars/acquisitions" ];
            entity[ "perceptions/radars/radar/activated" ] = false;
            entity[ "perceptions/radars/tapping/activated" ] = false;
            entity[ "perceptions/radars/tapping-radar/activated" ] = false;
            entity[ "perceptions/localized-radars/radar" ];
            entity[ "perceptions/localized-radars/tapping" ];
            entity[ "perceptions/localized-radars/tapping-radar" ];
            entity[ "perceptions/record-mode/activated" ] = false;
            core::Model& component = entity[ "components"].AddElement();
            component[ "type/sensor-rotation-angle" ] = 3;
            core::Model& sensor = component[ "sensors" ].AddElement();
            sensor[ "height" ] = 0;
            sensor[ "type" ] = "sensor-type";
            component[ "radars" ].AddElement()[ "type" ] = "my-radar";
        }
        const unsigned int identifier;
        const double maxPerceptionDistance;
        const double theoreticalDistance;
        const MT_Vector2D position;
        const MT_Vector2D direction;
        core::Model& entity;
    };
}
}

#endif // PERCEPTION_FIXTURE_H
