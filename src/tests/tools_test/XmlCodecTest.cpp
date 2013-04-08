// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/Codec.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: XmlCodecTest_DecodeTimeWithoutUnitIsInvalid
// Created: SBO 2009-11-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( XmlCodecTest_DecodeTimeWithoutUnitIsInvalid )
{
    const std::string input( "<data time='100'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "data" );
    {
        unsigned int value = 0;
        BOOST_CHECK_EQUAL( false, tools::ReadTimeAttribute( xis, "time", value ) );
    }
    {
        float value = 0;
        BOOST_CHECK_EQUAL( false, tools::ReadTimeAttribute( xis, "time", value ) );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlCodecTest_DecodeTimeWithSeconds
// Created: SBO 2009-11-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( XmlCodecTest_DecodeTimeWithSeconds )
{
    const std::string input( "<data time='100s'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "data" );
    {
        unsigned int value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 100u, value );
    }
    {
        float value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 100.f, value );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlCodecTest_DecodeTimeWithMinutes
// Created: SBO 2009-11-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( XmlCodecTest_DecodeTimeWithMinutes )
{
    const std::string input( "<data time='100m'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "data" );
    {
        unsigned int value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 100u * 60, value );
    }
    {
        float value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 100.f * 60, value );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlCodecTest_DecodeTimeWithHours
// Created: SBO 2009-11-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( XmlCodecTest_DecodeTimeWithHours )
{
    const std::string input( "<data time='100h'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "data" );
    {
        unsigned int value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 100u * 3600, value );
    }
    {
        float value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 100.f * 3600, value );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlCodecTest_DecodeTimeWithFloatingHours
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( XmlCodecTest_DecodeTimeWithFloatingHours )
{
    const std::string input( "<data time='0.5h'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "data" );
    {
        unsigned int value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 1800u, value );
    }
    {
        float value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 0.5f * 3600, value );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlCodecTest_DecodeTimeWithFloatingSeconds
// Created: MMC 2013-04-04
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( XmlCodecTest_DecodeTimeWithFloatingSeconds )
{
    const std::string input( "<data time='0.05s'/>" );
    xml::xistringstream xis( input ); xis >> xml::start( "data" );
    {
        unsigned int value = 0;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 0u, value );
    }
    {
        float value = 0.05f;
        BOOST_REQUIRE( tools::ReadTimeAttribute( xis, "time", value ) );
        BOOST_CHECK_EQUAL( 0.05f, value );
    }
}
