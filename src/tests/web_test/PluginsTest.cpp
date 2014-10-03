// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "web_test_pch.h"

#include "runtime/FileSystem.h"
#include "runtime/Helpers.h"
#include "runtime/PropertyTree.h"
#include "web/Configs.h"
#include "web/Plugins.h"

#include <cpplog/cpplog.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <map>

using namespace web;
using runtime::FileSystem;

BOOST_AUTO_TEST_CASE( plugin_directory_skips_invalid_root )
{
    cpplog::OstreamLogger log( std::cout );
    FileSystem fs( log );
    const std::string path = testOptions.GetDataPath( "missing" ).ToUTF8();
    Plugins plugins( fs, path );
    BOOST_CHECK( !plugins.Count() );
    BOOST_CHECK( plugins.GetNames( 0, INT_MAX ).empty() );
}

BOOST_AUTO_TEST_CASE( plugin_directory_parses )
{
    cpplog::OstreamLogger log( std::cout );
    FileSystem fs( log );
    const std::string path = testOptions.GetDataPath( "plugins" ).ToUTF8();
    static const char* valid_plugins[] =
    {
        "with spaces",
        "short",
        "two_libs",
        "bml",
    };
    Plugins plugins( fs, path );
    BOOST_CHECK_EQUAL( plugins.Count(), COUNT_OF( valid_plugins ) );
    for( size_t i = 0; i < COUNT_OF( valid_plugins ); ++i )
        BOOST_CHECK( plugins.Has( valid_plugins[i] ) );
    Plugins::T_Names names = plugins.GetNames( 0, INT_MAX );
    BOOST_CHECK_EQUAL( names.size(), COUNT_OF( valid_plugins ) );
    for( size_t i = 0; i < COUNT_OF( valid_plugins ); ++i )
        BOOST_CHECK( std::find( names.begin(), names.end(), valid_plugins[i] ) != names.end() );
}

namespace
{
template< typename T >
void CheckDefault( const T& actual, const typename T::value_type& expected )
{
    typename T::const_iterator it = actual.find( expected.first );
    BOOST_REQUIRE( it != actual.end() );
    BOOST_CHECK_EQUAL( it->second, expected.second );
}
}

BOOST_AUTO_TEST_CASE( plugin_parse_bml )
{
    cpplog::OstreamLogger log( std::cout );
    FileSystem fs( log );
    const std::string path = testOptions.GetDataPath( "plugins" ).ToUTF8();
    Plugins plugins( fs, path );
    const Plugins::T_Defaults expected = boost::assign::map_list_of
        ( "server/@url", "" )
        ( "server/@ssl", "false" )
        ( "server/@log", "true" )
        ( "server/proxy/@user", "" )
        ( "server/proxy/@pass", "" )
        ( "reports/@activate", "true" )
        ( "reports/@frequency", "30" );
    const Plugins::T_Defaults actual = plugins.GetDefaults( "bml" );
    BOOST_CHECK_EQUAL( expected.size(), actual.size() );
    BOOST_FOREACH( const Plugins::T_Defaults::value_type& value, expected )
        CheckDefault( actual, value );
    web::Tree tree;
    BOOST_FOREACH( const Plugins::T_Defaults::value_type& value, expected )
        if( value.first != "server/@log" )
            tree.put( "plugins.bml." + value.first, value.second );
    tree.put( "plugins.bml.server.log", "junk_data" );
    session::Config cfg;
    session::ReadConfig( cfg, plugins, tree );
    const session::PluginConfig& plugin = cfg.plugins.find( "bml" )->second;
    BOOST_CHECK_EQUAL( expected.size(), plugin.parameters.size() );
    BOOST_FOREACH( const Plugins::T_Defaults::value_type& value, expected )
        CheckDefault( plugin.parameters, std::make_pair( value.first,
                      value.first == "server/@log" ? "junk_data" : value.second ) );
}
