// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/GeneralConfig.h"

BOOST_AUTO_TEST_CASE( features_parsing )
{
    auto features = tools::SplitFeatures( "" );
    BOOST_CHECK_EQUAL( 0u, features.size() );
    auto s = tools::JoinFeatures( features );
    BOOST_CHECK_EQUAL( "", s );

    features = tools::SplitFeatures( "one-feature ; ; another" );
    BOOST_CHECK_EQUAL( 2u, features.size() );
    BOOST_CHECK_EQUAL( 1u, features.count( "one-feature" ) );
    BOOST_CHECK_EQUAL( 1u, features.count( "another" ) );

    features.clear();
    features.insert( "one-feature " );
    features.insert( " " );
    features.insert( " another" );
    s = tools::JoinFeatures( features );
    BOOST_CHECK_EQUAL( "another;one-feature", s );
}
