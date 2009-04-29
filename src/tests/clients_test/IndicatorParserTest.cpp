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
#include "indicators/IndicatorParser.h"

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestEmpty
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestEmpty )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    BOOST_CHECK_THROW( parser.Parse( "" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestBadSyntax
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestBadSyntax )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    BOOST_CHECK_THROW( parser.Parse( "test(" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNumberConstant
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNumberConstant )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleNumber_mocker.expects( once() ).with( eq( double( 12 ) ) );
    parser.Parse( "12" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestStringLiteral
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestStringLiteral )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleString_mocker.expects( once() ).with( eq< std::string >( "test" ) );
    parser.Parse( "'test'" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestVariable
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestVariable )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleVariable_mocker.expects( once() ).with( eq< std::string >( "MyVariable" ) );
    parser.Parse( "$MyVariable" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestFunctionCallNoParameter
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestFunctionCallNoParameter )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction" ), eq( 0u ) );
    parser.Parse( "MyFunction()" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestFunctionCallOneParameter
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestFunctionCallOneParameter )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleNumber_mocker.expects( once() ).with( eq( double( 12 ) ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction" ), eq( 1u ) );
    parser.Parse( "MyFunction( 12 )" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestFunctionCallManyParameters
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestFunctionCallManyParameters )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleNumber_mocker.expects( once() ).with( eq( double( 12 ) ) );
    handler.HandleString_mocker.expects( once() ).with( eq< std::string >( "test" ) );
    handler.HandleNumber_mocker.expects( once() ).with( eq( double( 51 ) ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction" ), eq( 3u ) );
    parser.Parse( "MyFunction( 12, 'test', 51 )" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNestedFunctionCall
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNestedFunctionCall )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyNestedFunction" ), eq( 0u ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction" ), eq( 1u ) );
    parser.Parse( "MyFunction( MyNestedFunction() )" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNestedFunctionsCallOrder
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNestedFunctionsCallOrder )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyNestedFunction3" ), eq( 0u ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyNestedFunction2" ), eq( 1u ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyNestedFunction1" ), eq( 0u ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction" ), eq( 2u ) );
    parser.Parse( "MyFunction( MyNestedFunction1(), MyNestedFunction2( MyNestedFunction3() ) )" );
    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser_TestNestedFunctionsWithMixedParameters
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorParser_TestNestedFunctionsWithMixedParameters )
{
    MockIndicatorGrammarHandler handler;
    IndicatorParser parser( handler );

    handler.HandleNumber_mocker.expects( once() ).with( eq( double( 12.f ) ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction3" ), eq( 0u ) );
    handler.HandleVariable_mocker.expects( once() ).with( eq< std::string >( "MyVariable" ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction2" ), eq( 2u ) );
    handler.HandleNumber_mocker.expects( once() ).with( eq( double( 42.f ) ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction1" ), eq( 1u ) );
    handler.HandleFunctionCall_mocker.expects( once() ).with( eq< std::string >( "MyFunction" ), eq( 3u ) );

    parser.Parse( "MyFunction( MyFunction1( 42 ), MyFunction2( $MyVariable, MyFunction3() ), 12 )" );
    handler.verify();
}
