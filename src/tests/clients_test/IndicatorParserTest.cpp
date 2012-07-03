// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "MockIndicatorGrammarHandler.h"
#include "indicators/FormulaParser.h"

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestEmpty
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestEmpty )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    BOOST_CHECK_THROW( parser.Parse( "" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestBadSyntax
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestBadSyntax )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    BOOST_CHECK_THROW( parser.Parse( "test(" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNumberConstant
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNumberConstant )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleNumber ).once().with( 12 );
    parser.Parse( "12" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestStringLiteral
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestStringLiteral )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleString ).once().with( "test" );
    parser.Parse( "'test'" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestVariable
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestVariable )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleVariable ).once().with( "MyVariable" );
    parser.Parse( "$MyVariable" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestFunctionCallNoParameter
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestFunctionCallNoParameter )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction", 0u );
    parser.Parse( "MyFunction()" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestFunctionCallOneParameter
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestFunctionCallOneParameter )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleNumber ).once().with( 12 );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction", 1u );
    parser.Parse( "MyFunction( 12 )" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestFunctionCallManyParameters
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestFunctionCallManyParameters )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleNumber ).once().with( 12 );
    MOCK_EXPECT( handler.HandleString ).once().with( "test" );
    MOCK_EXPECT( handler.HandleNumber ).once().with( 51 );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction", 3u );
    parser.Parse( "MyFunction( 12, 'test', 51 )" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNestedFunctionCall
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNestedFunctionCall )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyNestedFunction", 0u );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction", 1u );
    parser.Parse( "MyFunction( MyNestedFunction() )" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNestedFunctionsCallOrder
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNestedFunctionsCallOrder )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyNestedFunction3", 0u );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyNestedFunction2", 1u );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyNestedFunction1", 0u );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction", 2u );
    parser.Parse( "MyFunction( MyNestedFunction1(), MyNestedFunction2( MyNestedFunction3() ) )" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNestedFunctionsWithMixedParameters
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNestedFunctionsWithMixedParameters )
{
    MockIndicatorGrammarHandler handler;
    indicators::FormulaParser parser( handler );
    MOCK_EXPECT( handler.HandleNumber ).once().with( 12 );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction3", 0u );
    MOCK_EXPECT( handler.HandleVariable ).once().with( "MyVariable" );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction2", 2u );
    MOCK_EXPECT( handler.HandleNumber ).once().with( 42 );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction1", 1u );
    MOCK_EXPECT( handler.HandleFunctionCall ).once().with( "MyFunction", 3u );
    parser.Parse( "MyFunction( MyFunction1( 42 ), MyFunction2( $MyVariable, MyFunction3() ), 12 )" );
}
