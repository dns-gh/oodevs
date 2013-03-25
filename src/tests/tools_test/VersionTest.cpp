// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/Version.h"
#include <tools/Path.h>
#include "tools/zipextractor.h"

BOOST_AUTO_TEST_CASE( version_comparison )
{
    BOOST_CHECK( tools::CheckVersion( "", "trunk" ) );
    BOOST_CHECK( tools::CheckVersion( "", "1" ) );
    BOOST_CHECK( tools::CheckVersion( "1", "trunk" ) );
    BOOST_CHECK( tools::CheckVersion( "1", "1" ) );
    BOOST_CHECK( tools::CheckVersion( "1.1", "1.1" ) );
    BOOST_CHECK( ! tools::CheckVersion( "1.2", "1.1" ) );
    BOOST_CHECK( ! tools::CheckVersion( "2", "1.1" ) );
    BOOST_CHECK( tools::CheckVersion( "1.1", "1.2" ) );
    BOOST_CHECK( tools::CheckVersion( "1.1", "2" ) );
}
