// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "XmlChecks.h"
#include <qstring.h>
#include "preparation/IndicatorParser.h"
#include "preparation/IndicatorSerializer.h"
#include "preparation/IndicatorElement_ABC.h"
#include "preparation/IndicatorElementFactory.h"
#include "preparation/IndicatorPrimitives.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

namespace
{
    class SerializerFixture
    {
    public:
                 SerializerFixture() : factory_( primitives_ ) {}
        virtual ~SerializerFixture() {}

        void ParseAndCheck( const std::string& text, const std::string& expected )
        {
            xml::xostringstream xos;
            {
                IndicatorSerializer handler( factory_, xos );
                IndicatorParser parser( handler );
                parser.Parse( text );
            }
            BOOST_CHECK_XML_EQUAL( expected, xos.str() );
        }

    private:
        IndicatorPrimitives primitives_;
        IndicatorElementFactory factory_;
    };
}

BOOST_FIXTURE_TEST_SUITE( IndicatorSerializerTestSuite, SerializerFixture )

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestEmpty
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestEmpty )
{
    BOOST_CHECK_THROW( ParseAndCheck( "", "" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestExtractor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestExtractor )
{
    const std::string expected = 
        "<indicator>"
            "<extractor function='operational-state' id='1' type='list(key,float)'/>"
        "</indicator>";
    ParseAndCheck( "operational-state()", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestExtractorCaseInsensitive
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( IndicatorSerializer_TestExtractorCaseInsensitive, SerializerFixture )
{
    const std::string expected = 
        "<indicator>"
            "<extractor function='operational-state' id='1' type='list(key,float)'/>"
        "</indicator>";
    ParseAndCheck( "OpEraTionAl-StaTe()", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestExtractorWithParameters
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestExtractorWithParameters )
{
    const std::string expected = 
        "<indicator>"
            "<constant id='1' type='float' value='4'/>"
            "<constant id='2' type='float' value='5'/>"
            "<extractor equipments='1' function='equipments' id='3' states='2' type='list(key,equipments)'/>"
        "</indicator>";
    ParseAndCheck( "equipments( 4, 5 )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestNestedFunctions
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestNestedFunctions )
{
    const std::string expected = 
        "<indicator>"
            "<constant id='1' type='variable' value='$MyResources'/>"
            "<extractor dotations='1' function='resources' id='2' type='list(key,resources)'/>"
            "<constant id='3' type='variable' value='$MyUnits'/>"
            "<transformation function='domain' id='4' input='2' select='3' type='_1'/>"
            "<reduction function='sum' id='5' input='4' type='_1'/>"
        "</indicator>";
    ParseAndCheck( "sum( domain( resources( $MyResources ), $MyUnits ) )", expected );
}

BOOST_AUTO_TEST_SUITE_END()
