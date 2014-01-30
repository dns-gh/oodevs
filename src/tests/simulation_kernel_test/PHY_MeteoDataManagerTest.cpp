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
#include "simulation_kernel/Meteo/PHY_MeteoDataManager.h"
#include "simulation_kernel/MIL_Time_ABC.h"
#include <tools/Helpers.h>
#include <xeumeuleu/xml.hpp>
#include <boost/noncopyable.hpp>

BOOST_AUTO_TEST_CASE( phy_ephemeride_test )
{
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

BOOST_AUTO_TEST_CASE( phy_meteodatamanager )
{
    // This test may look useless but it actually creates a PHY_MeteoDataManager
    // without MIL_AgentServer *and without crashing*.
    // So who is the boss, hmm?
    xml::xistringstream xis(
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
    "<weather model-version=\"4.8.2\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:noNamespaceSchemaLocation=\"schemas/5.3.0/exercise/weather.xsd\">"
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>"
        "<theater>"
            "<wind direction=\"90\" speed=\"30\"/>"
            "<cloud-cover ceiling=\"9700\" density=\"4\" floor=\"800\"/>"
            "<temperature value=\"20\"/>"
            "<precipitation value=\"Crachin\"/>"
        "</theater>"
        "<local-weather/>"
    "</weather>"
    );
    tools::Path detectionFile = testOptions.GetDataPath(
            "../../data/terrains/Paris_Est/Detection/detection.dat" );
    PHY_MeteoDataManager meteo( xis, detectionFile, 0, 10 );
}
