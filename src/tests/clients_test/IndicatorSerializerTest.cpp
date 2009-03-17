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
#include <xeumeuleu/xml.h>

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestEmpty
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestEmpty )
{
    xml::xostringstream xos;
    IndicatorSerializer handler( xos );
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
            "<function name='MyFunction'/>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorSerializer handler( xos );
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
            "<function name='MyFunction'>"
                "<parameter value='12'/>"
                "<parameter value='42'/>"
            "</function>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorSerializer handler( xos );
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
            "<function name='MyFunction'>"
                "<parameter value='$MyVariable'/>"
            "</function>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorSerializer handler( xos );
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
            "<function name='MyFunction1'>"
                "<parameter value='42'/>"
            "</function>"
            "<function name='MyFunction3'/>"
            "<function name='MyFunction2'>"
                "<parameter value='$MyVariable'/>"
                "<parameter value='Function_MyFunction3'/>"
            "</function>"
            "<function name='MyFunction'>"
                "<parameter value='Function_MyFunction1'/>"
                "<parameter value='Function_MyFunction2'/>"
                "<parameter value='12'/>"
            "</function>"
        "</indicator>";
    xml::xostringstream xos;
    {
        IndicatorSerializer handler( xos );
        IndicatorParser parser( handler );
        parser.Parse( "MyFunction( MyFunction1( 42 ), MyFunction2( $MyVariable, MyFunction3() ), 12 )" );
    }
    BOOST_CHECK_XML_EQUAL( expected, xos.str() );
}
