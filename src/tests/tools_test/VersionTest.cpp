// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/VersionHelper.h"
#include <tools/Path.h>

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
    BOOST_CHECK( tools::CheckVersion( "1.1.0", "1.1" ) );
    BOOST_CHECK( tools::CheckVersion( "2.1", "2.1.0" ) );

    BOOST_CHECK( tools::CheckVersion( "5.2.0", "5.2.6", 2u ) );
    BOOST_CHECK( tools::CheckVersion( "5.2.2", "5.2.1", 2u ) );
    BOOST_CHECK( tools::CheckVersion( "5.2", "5.2.1", 2u ) );
    BOOST_CHECK( !tools::CheckVersion( "5.3.2", "5.2.1", 2u ) );
    BOOST_CHECK( !tools::CheckVersion( "5.2", "5.1.1", 2u ) );
}
