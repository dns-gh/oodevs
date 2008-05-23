// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "bml_plugin/Point.h"

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( BmlTest_BearingComputing )
{
    bml::Point point1( 45.0, 1.0 );
    bml::Point point2( 45.5, 1.5 );
    BOOST_CHECK_EQUAL( point1.ComputeBearing( point1 ), 0 );
    BOOST_CHECK_EQUAL( point1.ComputeBearing( point2 ), 45 );
    BOOST_CHECK_EQUAL( point2.ComputeBearing( point1 ), 180 + 45 );
}
