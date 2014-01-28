// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Meteo/PHY_Ephemeride.h"
#include "meteo/PHY_Lighting.h"
#include <tools/Helpers.h>
#include <xeumeuleu/xml.hpp>

BOOST_AUTO_TEST_CASE( phy_ephemeride_test )
{
    weather::PHY_Lighting::Initialize();

    const std::string invalid[] =
    {
        // Invalid day, night and moon lighting
        "<ephemerides day-lighting=\"invalid\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>",
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"invalid\" "
            "sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>",
        "<ephemerides moon=\"invalid\" sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>",
        // Invalid sunrise and sunset
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"25h30m0s\" sunset=\"21h30m0s\"/>",
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"7h30m0s\" sunset=\"21h60m0s\"/>",
    };
    for( size_t i = 0; i != COUNT_OF( invalid ); ++i )
    {
        xml::xistringstream xis( invalid[i] );
        BOOST_CHECK_THROW( ReadEphemeride( xis, 0 ), tools::Exception );
    }

    // Valid day/night instance
    const std::string e1 = "<ephemerides day-lighting=\"JourSansNuage\" "
        "night-lighting=\"NuitPleineLune\" sunrise=\"7h00m\" sunset=\"20h00m0s\"/>";
    xml::xistringstream xis( e1 );
    auto eph = ReadEphemeride( xis, 0 );
    BOOST_REQUIRE( eph );
    BOOST_CHECK( eph->IsNight() );

    // Check night switches
    BOOST_CHECK( !eph->UpdateNight( 3600*7 - 1 ) );
    BOOST_CHECK( eph->IsNight() );
    BOOST_CHECK( eph->UpdateNight( 3600*7 ) );
    BOOST_CHECK( !eph->IsNight() );

    BOOST_CHECK( !eph->UpdateNight( 3600*20 - 1 ) );
    BOOST_CHECK( !eph->IsNight() );
    BOOST_CHECK( eph->UpdateNight( 3600*20 ) );
    BOOST_CHECK( eph->IsNight() );

    // Moon instance for the fun
    const std::string e2 = "<ephemerides moon=\"JourSansNuage\" "
        "sunrise=\"7h00m\" sunset=\"20h00m0s\"/>";
    xml::xistringstream xis2( e2 );
    auto moon = ReadEphemeride( xis, 0 );
    BOOST_REQUIRE( moon );
}
