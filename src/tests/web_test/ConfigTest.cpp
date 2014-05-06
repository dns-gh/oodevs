// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test.h"

#include "runtime/PropertyTree.h"
#include "web/Configs.h"
#include "web/Plugins.h"

#include "../host_test/MockFileSystem.h"

using namespace property_tree;
using namespace web;

BOOST_AUTO_TEST_CASE( authorized_users_read_write )
{
    const std::string input =
"{"
"    \"restricted\": {"
"        \"enabled\": \"true\","
"        \"list\": {"
"            \"1\": {"
"                \"name\": \"uno\","
"                \"profiles\": ["
"                    \"admin\","
"                    \"user\""
"                ]"
"            },"
"            \"2\": {"
"                \"name\": \"duo\""
"            }"
"        }"
"    }"
"}";
    const auto check = []( const Tree& src ) -> session::Config {
        session::Config cfg;
        mocks::MockFileSystem fs;
        MOCK_EXPECT( fs.Walk ).with( mock::any, mock::any, mock::any );
        Plugins plugins( fs, "" );
        session::ReadConfig( cfg, plugins, src );
        BOOST_CHECK_EQUAL( cfg.restricted.enabled, true );
        session::User uno( 1, "uno" );
        uno.profiles.insert( "admin" );
        uno.profiles.insert( "user" );
        session::User duo( 2, "duo" );
        BOOST_CHECK_EQUAL( cfg.restricted.users.size(), 2u );
        BOOST_CHECK( uno == *cfg.restricted.users.find( uno ) );
        BOOST_CHECK( duo == *cfg.restricted.users.find( duo ) );
        return cfg;
    };
    const auto cfg = check( FromJson( input ) );
    Tree cycle;
    session::WriteConfig( cycle, cfg );
    check( cycle );
}
