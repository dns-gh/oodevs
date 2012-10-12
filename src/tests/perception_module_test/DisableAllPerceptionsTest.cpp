// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "PerceptionFixture.h"

namespace
{
    struct Fixture : public sword::perception::PerceptionFixture
    {
        void CheckDeactivated( core::Model& perception )
        {
            ExpectEffect( perception, sword::test::MakeModel( false ) );
        }
        void CheckElementRemoved( core::Model& perception )
        {
            perception[ 42u ];
            ExpectEffect( perception, sword::test::MakeModel( 42u, sword::test::MarkForRemove() ) );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( disable_all_perceptions_command_removes_localizations_and_deactivates_sensors, Fixture )
{
    core::Model& perceptions = model[ "entities" ][ identifier ][ "perceptions" ];
    CheckDeactivated( perceptions[ "peripherical-vision/activated" ] );
    CheckDeactivated( perceptions[ "sensor/activated" ] );
    CheckDeactivated( perceptions[ "scan/activated" ] );
    CheckDeactivated( perceptions[ "radars/radar/activated" ] );
    CheckDeactivated( perceptions[ "radars/tapping/activated" ] );
    CheckDeactivated( perceptions[ "radars/tapping-radar/activated" ] );
    CheckElementRemoved( perceptions[ "alat/reco" ] );
    ExpectEvent( "alat monitoring disabled", sword::test::MakeModel( "entity", identifier ) );
    CheckElementRemoved( perceptions[ "alat/monitoring" ] );
    CheckElementRemoved( perceptions[ "object-detection" ] );
    CheckElementRemoved( perceptions[ "urban" ] );
    CheckElementRemoved( perceptions[ "reco" ] );
    CheckElementRemoved( perceptions[ "flying-shell/zones" ] );
    CheckElementRemoved( perceptions[ "flying-shell/previous" ] );
    CheckElementRemoved( perceptions[ "recognition-point" ] );
    CheckElementRemoved( perceptions[ "radars/acquisitions" ] );
    CheckElementRemoved( perceptions[ "localized-radars/radar" ] );
    CheckElementRemoved( perceptions[ "localized-radars/tapping" ] );
    CheckElementRemoved( perceptions[ "localized-radars/tapping-radar" ] );
    StartCommand( "disable all perceptions", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
