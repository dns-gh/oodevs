// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "MT_Tools/MT_InterpolatedFunction.h"
#include <limits>

BOOST_AUTO_TEST_CASE( empty_interpolating_function_returns_default_value )
{
    MT_InterpolatedFunction< double > function;
    BOOST_CHECK_EQUAL( -1, function( 0 ) );
}

BOOST_AUTO_TEST_CASE( simple_interpolation_between_two_values )
{
    MT_InterpolatedFunction< double > function;
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 2 );
    function.AddNewPoint( 6, 10 );
    BOOST_CHECK_EQUAL( 0, function( 0 ) );
    BOOST_CHECK_EQUAL( 1, function( 1 ) );
    BOOST_CHECK_EQUAL( 2, function( 2 ) );
    BOOST_CHECK_EQUAL( 6, function( 4 ) );
    BOOST_CHECK_EQUAL( 10, function( 6 ) );
}

BOOST_AUTO_TEST_CASE( interpolation_values_are_sorted )
{
    MT_InterpolatedFunction< double > function;
    function.AddNewPoint( 2, 2 );
    function.AddNewPoint( 0, 0 );
    BOOST_CHECK_EQUAL( 0, function( 0 ) );
    BOOST_CHECK_EQUAL( 1, function( 1 ) );
    BOOST_CHECK_EQUAL( 2, function( 2 ) );
}

BOOST_AUTO_TEST_CASE( simple_interpolation_returns_first_value_if_before_first_boundary )
{
    MT_InterpolatedFunction< double > function;
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 2 );
    BOOST_CHECK_EQUAL( 0, function( -1 ) );
}

BOOST_AUTO_TEST_CASE( simple_interpolation_returns_first_value_if_after_last_boundary )
{
    MT_InterpolatedFunction< double > function;
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 2 );
    BOOST_CHECK_EQUAL( 2, function( 3 ) );
}

BOOST_AUTO_TEST_CASE( before_value_is_returned_if_set_and_before_first_boundary )
{
    MT_InterpolatedFunction< double > function;
    function.SetBeforeValue( -42 );
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 2 );
    BOOST_CHECK_EQUAL( -42, function( -1 ) );
}

BOOST_AUTO_TEST_CASE( after_value_is_returned_if_set_and_after_last_boundary )
{
    MT_InterpolatedFunction< double > function;
    function.SetAfterValue( -42 );
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 2 );
    BOOST_CHECK_EQUAL( -42, function( 3 ) );
}

BOOST_AUTO_TEST_CASE( function_retrieves_max_x_from_y_value )
{
    MT_InterpolatedFunction< double > function;
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 6 );
    function.AddNewPoint( 4, 0 );
    function.AddNewPoint( 42, 6 );
    BOOST_CHECK_EQUAL( 42, function.GetMaxYForX( 6 ) );
}

BOOST_AUTO_TEST_CASE( function_retrieves_min_x_from_y_value )
{
    MT_InterpolatedFunction< double > function;
    function.AddNewPoint( 0, 0 );
    function.AddNewPoint( 2, 6 );
    function.AddNewPoint( 4, 0 );
    function.AddNewPoint( 42, 6 );
    BOOST_CHECK_EQUAL( 2, function.GetMinYForX( 6 ) );
}
