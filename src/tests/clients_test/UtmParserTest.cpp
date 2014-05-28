// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "clients_gui/UtmParser.h"
#include "clients_kernel/CoordinateConverter.h"
#include "ENT/ENT_Enums.h"

namespace
{
    const geometry::Point2f position( -4556109.5f, -3237370.8f );

    void CheckParse( const gui::UtmParser& parser, const QStringList& content )
    {
        geometry::Point2f result;
        QStringList hint;
        BOOST_CHECK( parser.Parse( content, result, hint, false ) );
        BOOST_CHECK_EQUAL( position, result );
    }
}

BOOST_AUTO_TEST_CASE( UtmParserTest )
{
    const kernel::CoordinateConverter converter( eCoordinateSystem_SanC );
    const gui::UtmParser parser( converter, eCoordinateSystem_SanC );
    {
        CheckParse( parser, QStringList() << "21J" << "595711" << "6625169" );
        CheckParse( parser, QStringList() << " 21J " << " 595711 " << " 6625169 " );
    }
    BOOST_CHECK_EQUAL( "21J 595711 6625168", parser.GetStringPosition( position ) );
    BOOST_CHECK_EQUAL( QStringList() << "21J" << "595711" << "6625168", parser.Split( "21J5957116625168" ) );
    BOOST_CHECK_EQUAL( QStringList() << "21J" << "595711" << "6625168", parser.Split( "21J 595711 6625168" ) );
}
