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
#include <boost/assign/list_of.hpp>
#pragma warning( push, 0 )
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#pragma warning( pop )

using namespace tools;

BOOST_AUTO_TEST_CASE( map_inserts_value )
{
    tools::Map< int, int > map;
    int& i = map[ 0 ];
    BOOST_CHECK_EQUAL( 0, i );
    i = 3;
    BOOST_CHECK_EQUAL( 3, map[ 0 ] );
}

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
    tools::Map< int, int > map = boost::assign::map_list_of( 2, 1 )( 1, 2 );
    BOOST_REQUIRE_EQUAL( 2u, map.size() );
    auto it = map.begin();
    BOOST_REQUIRE( it != map.end() );
    BOOST_CHECK_EQUAL( 1, it->second );
    ++it;
    BOOST_REQUIRE( it != map.end() );
    BOOST_CHECK_EQUAL( 2, it->second );
}

BOOST_AUTO_TEST_CASE( map_finds_elements_in_insertion_order )
{
    tools::Map< int, int > map = boost::assign::map_list_of( 1, 2 )( 2, 3 )( 3, 4 );
    BOOST_REQUIRE_EQUAL( 3u, map.size() );
    BOOST_CHECK( map.find( 42 ) == map.end() );
    BOOST_CHECK( map.find( 1 ) == map.begin() );
    BOOST_CHECK( map.find( 2 ) == map.begin() + 1 );
    BOOST_CHECK( map.find( 3 ) == map.begin() + 2 );
}

BOOST_AUTO_TEST_CASE( const_map_finds_elements_in_insertion_order )
{
    const tools::Map< int, int > map = boost::assign::map_list_of( 1, 2 )( 2, 3 )( 3, 4 );
    BOOST_REQUIRE_EQUAL( 3u, map.size() );
    BOOST_CHECK( map.find( 42 ) == map.end() );
    BOOST_CHECK( map.find( 1 ) == map.begin() );
    BOOST_CHECK( map.find( 2 ) == map.begin() + 1 );
    BOOST_CHECK( map.find( 3 ) == map.begin() + 2 );
}

BOOST_AUTO_TEST_CASE( map_finds_const_elements_in_insertion_order )
{
    tools::Map< const int, const int > map = boost::assign::map_list_of( 1, 2 )( 2, 3 )( 3, 4 );
    BOOST_REQUIRE_EQUAL( 3u, map.size() );
    BOOST_CHECK( map.find( 42 ) == map.end() );
    BOOST_CHECK( map.find( 1 ) == map.begin() );
    BOOST_CHECK( map.find( 2 ) == map.begin() + 1 );
    BOOST_CHECK( map.find( 3 ) == map.begin() + 2 );
}

BOOST_AUTO_TEST_CASE( map_supports_swap )
{
    tools::Map< int, int > map = boost::assign::map_list_of( 1, 1 );
    BOOST_REQUIRE_EQUAL( 1u, map.size() );
    tools::Map< int, int >().swap( map );
}

BOOST_AUTO_TEST_CASE( map_is_comparable )
{
    tools::Map< int, int > map1 = boost::assign::map_list_of( 1, 1 );
    tools::Map< int, int > map2 = boost::assign::map_list_of( 1, 2 );
    BOOST_CHECK( map1 == map1 );
    BOOST_CHECK( map1 != map2 );
}

BOOST_AUTO_TEST_CASE( map_is_serializable )
{
    std::stringstream s;
    {
        tools::Map< int, int > map;
        map[ 1 ] = 2;
        boost::archive::text_oarchive a( s );
        a & map;
    }
    {
        tools::Map< int, int > map;
        boost::archive::text_iarchive a( s );
        a & map;
        BOOST_REQUIRE_EQUAL( 1u, map.size() );
        BOOST_CHECK_EQUAL( 2, map[ 1 ] );
    }
}

namespace
{
    struct convertible
    {
        operator int() const
        {
            return 0;
        }
    };
}

BOOST_AUTO_TEST_CASE( map_supports_type_conversion_when_inserting )
{
    convertible c;
    tools::Map< int, int > map;
    map.insert( std::make_pair( c, c ) );
}
