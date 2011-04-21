// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_test_pch.h"
#include "frontend/CreateSession.h"

// -----------------------------------------------------------------------------
// Name: SessionOption_SetOptionTest
// Created: RPD 2011-03-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SessionOption_SetOptionTest )
{
    frontend::CreateSession session( "dummy path" );
    session.SetDefaultValues();
    session.SetOption( "session/config/dispatcher/plugins/shield", "" );
    BOOST_CHECK( session.HasOption( "session/config/dispatcher/plugins/shield" ) );
    session.SetOption( "session/config/dispatcher/plugins/shield/@server", 33001 );
    BOOST_CHECK( session.HasOption( "session/config/dispatcher/plugins/shield/@server" ) );
    BOOST_CHECK_EQUAL( 33001, session.GetOption< int >( "session/config/dispatcher/plugins/shield/@server" ) );
}

// -----------------------------------------------------------------------------
// Name: SessionOption_RemoveOptionTest
// Created: RPD 2011-03-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SessionOption_RemoveOptionTest )
{
    frontend::CreateSession session( "dummy path" );
    session.SetDefaultValues();
	session.SetOption( "session/config/dispatcher/plugins/shield", "" );
    BOOST_CHECK( session.HasOption( "session/config/dispatcher/plugins/recorder" ) );
    BOOST_CHECK( session.HasOption( "session/config/dispatcher/plugins/shield" ) );
    session.RemoveOption( "session/config/dispatcher/plugins/recorder" );
    BOOST_CHECK( !session.HasOption( "session/config/dispatcher/plugins/recorder" ) );
    BOOST_CHECK( session.HasOption( "session/config/dispatcher/plugins/shield" ) );
    BOOST_CHECK( session.HasOption( "session/config/dispatcher/plugins" ) );
}