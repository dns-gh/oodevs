// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test_pch.h"

#include "host/Crypt.h"

using namespace host;

namespace
{
    struct Fixture
    {
        Fixture()
        {
            // NOTHING
        }

        const Crypt crypt;
    };
}

BOOST_FIXTURE_TEST_CASE( crypt_validates, Fixture )
{
    const std::string text = crypt.Hash( "admin" );
    BOOST_CHECK( crypt.Validate( "admin", text ) );
}

BOOST_FIXTURE_TEST_CASE( crypt_invalidates, Fixture )
{
    const std::string text = crypt.Hash( "admin" );
    BOOST_CHECK( !crypt.Validate( "admin", text.substr( 0, text.size() - 1 ) ) );
    BOOST_CHECK( !crypt.Validate( "admin", text + " " ) );
    BOOST_CHECK( !crypt.Validate( "admin", "admin" ) );
    BOOST_CHECK( !crypt.Validate( "admin", "" ) );
}
