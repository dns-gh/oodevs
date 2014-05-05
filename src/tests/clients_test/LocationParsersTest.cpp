// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#pragma warning( push, 0 )
#include <QtCore/qstringlist.h>
#pragma warning( pop )
#include "clients_kernel/CoordinateConverter.h"
#include "clients_gui/Wgs84DmsParser.h"

namespace
{

// Deal with Parse() output not being proper unicode because the default
// translated pattern is invalid. See Wgs84DmsParser for details.
std::string CleanupCoords( QString coords )
{
    // Qt apparently converts "unknown" code points to \x3f (?)
    coords.replace( '\x3f', '?' );
    return coords.toStdString();
}

} // namespace

BOOST_AUTO_TEST_CASE( Wgs84DmsFormatTest )
{
    kernel::CoordinateConverter converter;
    gui::Wgs84DmsParser parser( converter );
    QStringList coords, hint;
    geometry::Point2f pos;

    // Two negative greater than zero
    coords << "12N 25 35 " << " 13W 26 36";
    BOOST_CHECK( parser.Parse( coords, pos, hint, false ) );
    BOOST_CHECK_EQUAL( CleanupCoords( hint.join( " " ) ), "12? 25' 35.00 N 13? 26' 36.00 W" );

    // One positive, one small negative (below 1 degree)
    hint.clear();
    coords.clear();
    coords << "0N 25 35 " << " 0W 26 36";
    BOOST_CHECK( parser.Parse( coords, pos, hint, false ) );
    BOOST_CHECK_EQUAL( CleanupCoords( hint.join( " " ) ), "00? 25' 35.00 N 00? 26' 36.00 W" );
}

