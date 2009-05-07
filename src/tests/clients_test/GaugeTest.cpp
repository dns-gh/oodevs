// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "indicators/GaugeNormalizer.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: Gauge_TestNormalizationIntervalBoundariesAreImplicit
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Gauge_TestNormalizationIntervalBoundariesAreImplicit )
{
    const std::string initialisation = 
        "<root>"
            "<interval max='25' key='0'/>"
            "<interval min='25' key='1'/>"
        "</root>";
    xml::xistringstream xis( initialisation ); xis >> xml::start( "root" );
    indicators::GaugeNormalizer normalizer( xis );
    BOOST_CHECK_EQUAL( 0, normalizer.Normalize( 10.f ) );
    BOOST_CHECK_EQUAL( 1, normalizer.Normalize( 30.f ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge_TestNormalizationIntervalsMatchLessOrEqual
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Gauge_TestNormalizationIntervalsMatchLessOrEqual )
{
    const std::string initialisation = 
        "<root>"
            "<interval max='25' key='0'/>"
            "<interval min='25' key='1'/>"
        "</root>";
    xml::xistringstream xis( initialisation ); xis >> xml::start( "root" );
    indicators::GaugeNormalizer normalizer( xis );
    BOOST_CHECK_EQUAL( 0, normalizer.Normalize( 25.f ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge_TestNormalizationIntervalsCanBeDiscontinuous
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Gauge_TestNormalizationIntervalsCanBeDiscontinuous )
{
    const std::string initialisation = 
        "<root>"
            "<interval max='10' key='0'/>"
            "<interval min='25' max='75' key='2'/>"
            "<interval min='80' key='1'/>"
        "</root>";
    xml::xistringstream xis( initialisation ); xis >> xml::start( "root" );
    indicators::GaugeNormalizer normalizer( xis );
    BOOST_CHECK_EQUAL( 2, normalizer.Normalize( 50.f ) );
    BOOST_CHECK_EQUAL( 25.f, normalizer.Normalize( 25.f ) );
    BOOST_CHECK_EQUAL( 80.f, normalizer.Normalize( 80.f ) );
}