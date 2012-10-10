// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "clients_kernel/ASCExtractor.h"
#pragma warning( push )
#pragma warning( disable : 4996 )

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: extract_ascii_grid
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_ascii_grid )
{
    kernel::ASCExtractor extractor( BOOST_RESOLVE( "ascii_test.asc" ) );

    BOOST_CHECK_EQUAL( extractor.GetNoValueData(), -9999 );
    BOOST_CHECK_EQUAL( extractor.GetMaximumValue(), 100 );
    BOOST_CHECK_EQUAL( extractor.GetMinimumValue(), 1 );
    BOOST_CHECK_EQUAL( extractor.GetCols(), 4 );
    BOOST_CHECK_EQUAL( extractor.GetRows(), 6 );

    const geometry::Point2d origin = extractor.GetOrigin();
    BOOST_CHECK_EQUAL( origin.X(), 1 );
    BOOST_CHECK_EQUAL( origin.Y(), 302 );
    const geometry::Point2d pixelSire = extractor.GetPixelSize();
    BOOST_CHECK_EQUAL( pixelSire.X(), 50 );
    BOOST_CHECK_EQUAL( pixelSire.Y(), -50 );

    kernel::ASCExtractor::T_Tiles tiles = extractor.GetTiles();
    BOOST_CHECK_EQUAL( tiles.size(), 20u );
}

// -----------------------------------------------------------------------------
// Name: extract_ascii_grid
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_ascii_grid_with_size_factor )
{
    kernel::ASCExtractor extractor( BOOST_RESOLVE( "ascii_test.asc" ), 2 );

    BOOST_CHECK_EQUAL( extractor.GetNoValueData(), -9999 );
    BOOST_CHECK_EQUAL( extractor.GetMaximumValue(), 45.25 );
    BOOST_CHECK_EQUAL( extractor.GetMinimumValue(), 1 );
    BOOST_CHECK_EQUAL( extractor.GetCols(), 4 );
    BOOST_CHECK_EQUAL( extractor.GetRows(), 6 );

    const geometry::Point2d origin = extractor.GetOrigin();
    BOOST_CHECK_EQUAL( origin.X(), 1 );
    BOOST_CHECK_EQUAL( origin.Y(), 302 );
    const geometry::Point2d pixelSire = extractor.GetPixelSize();
    BOOST_CHECK_EQUAL( pixelSire.X(), 50 );
    BOOST_CHECK_EQUAL( pixelSire.Y(), -50 );

    kernel::ASCExtractor::T_Tiles tiles = extractor.GetTiles();
    BOOST_CHECK_EQUAL( tiles.size(), 6u );
}
