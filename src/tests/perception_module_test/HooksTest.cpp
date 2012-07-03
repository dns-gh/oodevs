// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "ModuleFixture.h"

BOOST_FIXTURE_TEST_CASE( is_using_active_radar_hook_checks_if_one_its_radars_is_activated, sword::perception::ModuleFixture )
{
    core::Model entity;
    entity[ "perceptions/radars/radar/activated" ] = false;
    entity[ "perceptions/radars/tapping/activated" ] = false;
    entity[ "perceptions/radars/tapping-radar/activated" ] = false;
    entity[ "perceptions/localized-radars/radar/activated" ] = false;
    entity[ "perceptions/localized-radars/tapping/activated" ] = false;
    entity[ "perceptions/localized-radars/tapping-radar/activated" ] = false;
    entity[ "perceptions/flying-shell/activated" ] = false;
    BOOST_CHECK( !IsUsingActiveRadar( core::Convert( &entity ) ) );
    entity[ "perceptions/localized-radars/tapping-radar/activated" ] = true;
    BOOST_CHECK( IsUsingActiveRadar( core::Convert( &entity ) ) );
}
