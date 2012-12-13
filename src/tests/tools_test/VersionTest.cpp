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
#include <boost/tuple/tuple.hpp>

BOOST_AUTO_TEST_CASE( version_comparison )
{
    typedef boost::tuple< std::string, std::string, bool > Case;
    static const Case tests[] =
    {
        // Empty inputs
        Case( "",  "trunk", true ),
        Case( "",  "1", true ),
        Case( "1",  "trunk", true ),
        // Regular inputs
        Case( "1",  "1", true ),
        Case( "1.1",  "1.1", true ),
        Case( "1.2",  "1.1", false ),
        Case( "2",  "1.1", false ),
        Case( "1.1",  "1.2", true ),
        Case( "1.1",  "2", true ),
    };
    for( size_t i = 0; i != sizeof( tests )/sizeof( *tests ); ++i )
    {
        std::string v0 = tests[i].get<0>();
        std::string v1 = tests[i].get<1>();
        bool ret = tools::CheckVersion( v0, v1 );
        bool expected = tests[i].get<2>();
        BOOST_CHECK_MESSAGE( expected == ret,
            "comparing " << v0 << " and " << v1 << ", expecting " << expected
            << " got " << ret );
    }
}

