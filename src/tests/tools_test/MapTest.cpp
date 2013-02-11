// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/Map.h"

using namespace tools;

BOOST_AUTO_TEST_CASE( map_clear_removes_all_elements )
{
    tools::Map< int, int > map;
    BOOST_REQUIRE_EQUAL( 0u, map.size() );
    BOOST_REQUIRE( map.empty() );
    map.insert( std::make_pair( 1, 2 ) );
    BOOST_REQUIRE_EQUAL( 1u, map.size() );
    BOOST_REQUIRE( ! map.empty() );
    map.clear();
    BOOST_REQUIRE_EQUAL( 0u, map.size() );
    BOOST_REQUIRE( map.empty() );
}

BOOST_AUTO_TEST_CASE( map_insert_adds_elements )
{
    tools::Map< int, int > map;
    auto result = map.insert( std::make_pair( 1, 2 ) );
    BOOST_CHECK( result.second );
    BOOST_CHECK( result.first == map.begin() );
    result = map.insert( std::make_pair( 1, 2 ) );
    BOOST_CHECK( ! result.second );
    BOOST_CHECK( result.first == map.begin() );
    BOOST_REQUIRE_EQUAL( 1u, map.size() );
}

BOOST_AUTO_TEST_CASE( map_erase_removes_elements )
{
    tools::Map< int, int > map;
    {
        auto it = map.insert( std::make_pair( 1, 2 ) ).first;
        BOOST_REQUIRE( ! map.empty() );
        it = map.erase( it );
        BOOST_REQUIRE( map.empty() );
        BOOST_CHECK( it == map.begin() );
    }
    {
        auto it = map.insert( std::make_pair( 1, 2 ) ).first;
        BOOST_REQUIRE( ! map.empty() );
        BOOST_CHECK_EQUAL( 1u, map.erase( 1 ) );
        BOOST_REQUIRE( map.empty() );
        BOOST_CHECK_EQUAL( 0u, map.erase( 1 ) );
    }
}

BOOST_AUTO_TEST_CASE( map_iterates_in_the_insertion_order )
{
    tools::Map< int, int > map;
    map[ 2 ] = 1;
    map[ 1 ] = 2;
    BOOST_REQUIRE_EQUAL( 2u, map.size() );
    auto it = map.begin();
    BOOST_REQUIRE( it != map.end() );
    BOOST_CHECK_EQUAL( 1, it->second );
    ++it;
    BOOST_REQUIRE( it != map.end() );
    BOOST_CHECK_EQUAL( 2, it->second );
}

BOOST_AUTO_TEST_CASE( map_find_finds_elements_in_insertion_order )
{
    tools::Map< int, int > map;
    map[ 1 ] = 2;
    map[ 2 ] = 3;
    map[ 3 ] = 4;
    BOOST_CHECK( map.find( 42 ) == map.end() );
    BOOST_CHECK( map.find( 1 ) == map.begin() );
    BOOST_CHECK( map.find( 2 ) == map.begin() + 1 );
    BOOST_CHECK( map.find( 3 ) == map.begin() + 2 );
}
