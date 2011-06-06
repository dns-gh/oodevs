// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include <boost/regex.hpp>

BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing )
{
    const boost::regex expression( "(sde|postgresql):\\/\\/(\\w+):(\\w+)@(\\w+):(\\d+)\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    const std::string url( "sde://user:pass@localhost:4242/database.schema" );
    BOOST_REQUIRE( boost::regex_match( url.c_str(), matches, expression ) );
    BOOST_CHECK_EQUAL( matches[ 0 ], url );
    BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
    BOOST_CHECK_EQUAL( matches[ 3 ], "pass" );
    BOOST_CHECK_EQUAL( matches[ 4 ], "localhost" );
    BOOST_CHECK_EQUAL( matches[ 5 ], "4242" );
    BOOST_CHECK_EQUAL( matches[ 6 ], "database" );
    BOOST_CHECK_EQUAL( matches[ 7 ], "schema" );
}

BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_2 )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)" );
    boost::cmatch matches;
    const std::string url( "sde://user:08/database" );
    //BOOST_REQUIRE( boost::regex_match( url.c_str(), matches, expression ) );
    //BOOST_CHECK_EQUAL( matches[ 0 ], url );
    //BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
    //BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
    //BOOST_CHECK_EQUAL( matches[ 4 ], "database" );
}

BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_Port )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@(\\w+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    const std::string url( "sde://user:08@localhost/database.schema" );
    BOOST_REQUIRE( boost::regex_match( url.c_str(), matches, expression ) );
    BOOST_CHECK_EQUAL( matches[ 0 ], url );
    BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
    BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
    BOOST_CHECK_EQUAL( matches[ 4 ], "localhost" );
    BOOST_CHECK_EQUAL( matches[ 5 ], "" );
    BOOST_CHECK_EQUAL( matches[ 6 ], "" );
    BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
    BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
    BOOST_CHECK_EQUAL( matches.size(), 9u );
}

BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_IpAndPort )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@([\\w\\-\\.]+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    const std::string url( "sde://user:08@172.0.65.2/database.schema" );
    BOOST_REQUIRE( boost::regex_match( url.c_str(), matches, expression ) );
    BOOST_CHECK_EQUAL( matches[ 0 ], url );
    BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
    BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
    BOOST_CHECK_EQUAL( matches[ 4 ], "172.0.65.2" );
    BOOST_CHECK_EQUAL( matches[ 5 ], "" );
    BOOST_CHECK_EQUAL( matches[ 6 ], "" );
    BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
    BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
    BOOST_CHECK_EQUAL( matches.size(), 9u );
}

BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_HostAndPort )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@([\\w\\-\\.]+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    const std::string url( "sde://user:08@my-host-25/database.schema" );
    BOOST_REQUIRE( boost::regex_match( url.c_str(), matches, expression ) );
    BOOST_CHECK_EQUAL( matches[ 0 ], url );
    BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
    BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
    BOOST_CHECK_EQUAL( matches[ 4 ], "my-host-25" );
    BOOST_CHECK_EQUAL( matches[ 5 ], "" );
    BOOST_CHECK_EQUAL( matches[ 6 ], "" );
    BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
    BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
    BOOST_CHECK_EQUAL( matches.size(), 9u );
}

BOOST_AUTO_TEST_CASE( EsriTest_ConnectionParsing_PortOptional )
{
    const boost::regex expression( "(sde):\\/\\/(\\w+):(\\w+)@(\\w+)(:(\\d+)){0,1}\\/(\\w*).(\\w*)" );
    boost::cmatch matches;
    const std::string url( "sde://user:08@localhost:4242/database.schema" );
    BOOST_REQUIRE( boost::regex_match( url.c_str(), matches, expression ) );
    BOOST_CHECK_EQUAL( matches[ 0 ], url );
    BOOST_CHECK_EQUAL( matches[ 2 ], "user" );
    BOOST_CHECK_EQUAL( matches[ 3 ], "08" );
    BOOST_CHECK_EQUAL( matches[ 4 ], "localhost" );
    BOOST_CHECK_EQUAL( matches[ 5 ], ":4242" );
    BOOST_CHECK_EQUAL( matches[ 6 ], "4242" );
    BOOST_CHECK_EQUAL( matches[ 7 ], "database" );
    BOOST_CHECK_EQUAL( matches[ 8 ], "schema" );
    BOOST_CHECK_EQUAL( matches.size(), 9u );
}
