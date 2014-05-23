// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "clients_gui/MgrsParser.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "ENT/ENT_Enums.h"

namespace
{
    const geometry::Point2f position( -4556099.5f, -3237331.5f );

    void CheckParse( const gui::MgrsParser& parser, const QStringList& content )
    {
        geometry::Point2f result;
        QStringList hint;
        BOOST_CHECK( parser.Parse( content, result, hint, false ) );
        BOOST_CHECK_EQUAL( position, result );
    }
}

BOOST_AUTO_TEST_CASE( MgrsParserTest )
{
    int argc = 1;
    char* argv = "";
    QApplication app(argc, &argv);
    kernel::Controllers controllers;
    const kernel::CoordinateConverter converter;
    const gui::MgrsParser parser( controllers, converter );
    {
        CheckParse( parser, QStringList() << "21JWG" << "95712" << "25175" );
        CheckParse( parser, QStringList() << " 21JWG " << " 95712 " << " 25175 " );
    }
    BOOST_CHECK_EQUAL( "21JWG9571325175", parser.GetStringPosition( position ) );
    BOOST_CHECK_EQUAL( QStringList() << "21JWG" << "95660" << "25131", parser.Split( "21JWG9566025131" ) );
    BOOST_CHECK_EQUAL( QStringList() << "21JWG" << "95660" << "25131", parser.Split( "21JWG 95660 25131" ) );
}
