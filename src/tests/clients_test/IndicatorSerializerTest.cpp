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
#include "preparation/IndicatorParser.h"
#include "preparation/IndicatorSerializer.h"
#include "preparation/IndicatorElement_ABC.h"
#include "preparation/IndicatorElementFactory.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestEmpty
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestEmpty )
{
    xml::xostringstream xos;
    IndicatorElementFactory factory;
    IndicatorSerializer handler( factory, xos );
    IndicatorParser parser( handler );
    BOOST_CHECK_THROW( parser.Parse( "" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestFunctionCallWithoutParameter
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestFunctionCallWithoutParameter )
{
    const std::string expected = 
        "<indicator>"
            "<function id='1' name='MyFunction'/>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorElementFactory factory;
        IndicatorSerializer handler( factory, xos );
        IndicatorParser parser( handler );
        parser.Parse( "MyFunction()" );
    }
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestFunctionCallWithParameters
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestFunctionCallWithParameters )
{
    const std::string expected = 
        "<indicator>"
            "<constant id='1' value='12'/>"
            "<constant id='2' value='42'/>"
            "<function id='3' input='1,2' name='MyFunction'/>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorElementFactory factory;
        IndicatorSerializer handler( factory, xos );
        IndicatorParser parser( handler );
        parser.Parse( "MyFunction( 12, 42 )" );
    }
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestVariable
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestVariable )
{
    const std::string expected = 
        "<indicator>"
            "<constant id='1' value='$MyVariable'/>"
            "<function id='2' input='1' name='MyFunction'/>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorElementFactory factory;
        IndicatorSerializer handler( factory, xos );
        IndicatorParser parser( handler );
        parser.Parse( "MyFunction( $MyVariable )" );
    }
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestNestedFunctionCalls
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestNestedFunctionCalls )
{
    const std::string expected = 
        "<indicator>"
            "<constant id='1' value='42'/>"
            "<function id='2' input='1' name='MyFunction1'/>"
            "<function id='4' name='MyFunction3'/>"
            "<constant id='3' value='$MyVariable'/>"
            "<function id='5' input='3,4' name='MyFunction2'/>"
            "<constant id='6' value='12'/>"
            "<function id='7' input='2,5,6' name='MyFunction'/>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorElementFactory factory;
        IndicatorSerializer handler( factory, xos );
        IndicatorParser parser( handler );
        parser.Parse( "MyFunction( MyFunction1( 42 ), MyFunction2( $MyVariable, MyFunction3() ), 12 )" );
    }
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
