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

namespace
{
    void CheckData( const kernel::ASCExtractor& extractor )
    {
        BOOST_CHECK_EQUAL( extractor.GetNoValueData(), -9999 );
        BOOST_CHECK_EQUAL( extractor.GetMaximumValue(), 100 );
        BOOST_CHECK_EQUAL( extractor.GetCols(), 4 );
        BOOST_CHECK_EQUAL( extractor.GetRows(), 6 );

        const geometry::Point2d origin = extractor.GetOrigin();
        BOOST_CHECK_EQUAL( origin.X(), 1 );
        BOOST_CHECK_EQUAL( origin.Y(), 302 );
        const geometry::Point2d pixelSire = extractor.GetPixelSize();
        BOOST_CHECK_EQUAL( pixelSire.X(), 50 );
        BOOST_CHECK_EQUAL( pixelSire.Y(), -50 );

        const kernel::ASCExtractor::T_Values& values = extractor.GetValues();
        BOOST_CHECK_EQUAL( values.size(), 24u );
    }
}

// -----------------------------------------------------------------------------
// Name: extract_ascii_grid
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_ascii_grid )
{
    CheckData( kernel::ASCExtractor( BOOST_RESOLVE( "ascii_test.asc" ) ) );
}

// -----------------------------------------------------------------------------
// Name: extract_ascii_grid
// Created: LGY 2012-03-10
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( extract_ascii_grid_with_external_prj )
{
    CheckData( kernel::ASCExtractor( BOOST_RESOLVE( "ascii_test.asc" ), BOOST_RESOLVE( "ascii_test.prj" ) ) );
}
