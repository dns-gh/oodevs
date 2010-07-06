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
#include <xeumeuleu/xml.hpp>

namespace
{
    class TestReader
    {
    public:
        explicit TestReader( xml::xistream& xis )
        {
            xis >> xml::optional() >> xml::start( "tests" )
                    >> xml::list( "test", *this, &TestReader::ReadTest )
                >> xml::end();
        }
        void ReadTest( xml::xistream& xis )
        {
            throw std::runtime_error( __FUNCTION__ );
        }
    };
}

BOOST_AUTO_TEST_CASE( XmlTest_ThrowInList ) // $$$$ _RC_ LGY 2010-07-06: useless
{
    xml::xistringstream xis( "<root>"
                             "    <tests>"
                             "        <test>"
                             "            <blabla/>"
                             "        </test>"
                             "        <test/>"
                             "    </tests>"
                             "</root>" );
    std::auto_ptr< TestReader > reader;
    BOOST_CHECK_THROW( reader.reset( new TestReader( xis >> xml::start( "root" ) ) ), std::runtime_error );
}

// -----------------------------------------------------------------------------
// Name: Gauge_TestNormalizationIntervalBoundariesAreImplicit
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Gauge_TestNormalizationIntervalBoundariesAreImplicit )
{
    xml::xistringstream xis( "<root>"
                             "    <interval max='25' key='0'/>"
                             "    <interval min='25' key='1'/>"
                             "</root>" );
    indicators::GaugeNormalizer normalizer( xis >> xml::start( "root" ) );
    BOOST_CHECK_EQUAL( 0.f, normalizer.Normalize( 10.f ) );
    BOOST_CHECK_EQUAL( 1.f, normalizer.Normalize( 30.f ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge_TestNormalizationIntervalsMatchLessOrEqual
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Gauge_TestNormalizationIntervalsMatchLessOrEqual )
{
    xml::xistringstream xis( "<root>"
                             "    <interval max='25' key='0'/>"
                             "    <interval min='25' key='1'/>"
                             "</root>" );
    indicators::GaugeNormalizer normalizer( xis >> xml::start( "root" ) );
    BOOST_CHECK_EQUAL( 0.f, normalizer.Normalize( 25.f ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge_TestNormalizationIntervalsCanBeDiscontinuous
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Gauge_TestNormalizationIntervalsCanBeDiscontinuous )
{
    xml::xistringstream xis( "<root>"
                             "    <interval max='10' key='0'/>"
                             "    <interval min='25' max='75' key='2'/>"
                             "    <interval min='80' key='1'/>"
                             "</root>") ;
    indicators::GaugeNormalizer normalizer( xis >> xml::start( "root" ) );
    BOOST_CHECK_EQUAL( 2, normalizer.Normalize( 50.f ) );
    BOOST_CHECK_EQUAL( 25.f, normalizer.Normalize( 25.f ) );
    BOOST_CHECK_EQUAL( 80.f, normalizer.Normalize( 80.f ) );
}
