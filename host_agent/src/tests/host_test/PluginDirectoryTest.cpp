// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/PluginDirectory.h"
#include "runtime/FileSystem.h"
#include <cpplog/cpplog.hpp>

using namespace host;
using namespace runtime;

BOOST_AUTO_TEST_CASE( plugin_directory_skips_invalid_root )
{
    cpplog::OstreamLogger log( std::cout );
    FileSystem fs( log );
    const std::string path = BOOST_RESOLVE( "missing" );
    PluginDirectory plugins( fs, path );
    BOOST_CHECK( !plugins.Count() );
    BOOST_CHECK( plugins.GetNames( 0, INT_MAX ).empty() );
}

#define COUNT_OF( X ) (sizeof(X)/sizeof*(X))

BOOST_AUTO_TEST_CASE( plugin_directory_parses )
{
    cpplog::OstreamLogger log( std::cout );
    FileSystem fs( log );
    const std::string path = BOOST_RESOLVE( "plugins" );
    static const char* valid_plugins[] =
    {
        "with spaces",
        "short",
        "two_libs"
    };
    PluginDirectory plugins( fs, path );
    BOOST_CHECK_EQUAL( plugins.Count(), COUNT_OF( valid_plugins ) );
    for( size_t i = 0; i < COUNT_OF( valid_plugins ); ++i )
        BOOST_CHECK( plugins.Has( valid_plugins[i] ) );
    PluginDirectory::T_Names names = plugins.GetNames( 0, INT_MAX );
    BOOST_CHECK_EQUAL( names.size(), COUNT_OF( valid_plugins ) );
    for( size_t i = 0; i < COUNT_OF( valid_plugins ); ++i )
        BOOST_CHECK( std::find( names.begin(), names.end(), valid_plugins[i] ) != names.end() );
}
