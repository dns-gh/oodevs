// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/Set.h"
#include <boost/assign/list_of.hpp>
#pragma warning( push, 0 )
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#pragma warning( pop )

using namespace tools;

BOOST_AUTO_TEST_CASE( set_clear_removes_all_elements )
{
    tools::Set< int > set;
    BOOST_REQUIRE_EQUAL( 0u, set.size() );
    BOOST_REQUIRE( set.empty() );
    set.insert( 1 );
    BOOST_REQUIRE_EQUAL( 1u, set.size() );
    BOOST_REQUIRE( ! set.empty() );
    set.clear();
    BOOST_REQUIRE_EQUAL( 0u, set.size() );
    BOOST_REQUIRE( set.empty() );
}

BOOST_AUTO_TEST_CASE( set_insert_adds_elements )
{
    tools::Set< int > set;
    auto result = set.insert( 1 );
    BOOST_CHECK( result.second );
    BOOST_CHECK( result.first == set.begin() );
    result = set.insert( 1 );
    BOOST_CHECK( ! result.second );
    BOOST_CHECK( result.first == set.begin() );
    BOOST_REQUIRE_EQUAL( 1u, set.size() );
}

BOOST_AUTO_TEST_CASE( set_erase_removes_elements )
{
    tools::Set< int > set;
    {
        auto it = set.insert( 1 ).first;
        BOOST_REQUIRE( ! set.empty() );
        it = set.erase( it );
        BOOST_REQUIRE( set.empty() );
        BOOST_CHECK( it == set.begin() );
    }
    {
        auto it = set.insert( 1 ).first;
        BOOST_REQUIRE( ! set.empty() );
        BOOST_CHECK_EQUAL( 1u, set.erase( 1 ) );
        BOOST_REQUIRE( set.empty() );
        BOOST_CHECK_EQUAL( 0u, set.erase( 1 ) );
    }
}

BOOST_AUTO_TEST_CASE( set_iterates_in_the_insertion_order )
{
    tools::Set< int > set = boost::assign::list_of( 2 )( 1 );
    BOOST_REQUIRE_EQUAL( 2u, set.size() );
    auto it = set.begin();
    BOOST_REQUIRE( it != set.end() );
    BOOST_CHECK_EQUAL( 2, *it );
    ++it;
    BOOST_REQUIRE( it != set.end() );
    BOOST_CHECK_EQUAL( 1, *it );
}

BOOST_AUTO_TEST_CASE( set_finds_elements_in_insertion_order )
{
    tools::Set< int > set = boost::assign::list_of( 1 )( 3 )( 2 );
    BOOST_REQUIRE_EQUAL( 3u, set.size() );
    BOOST_CHECK( set.find( 42 ) == set.end() );
    BOOST_CHECK( set.find( 1 ) == set.begin() );
    BOOST_CHECK( set.find( 2 ) == set.begin() + 2 );
    BOOST_CHECK( set.find( 3 ) == set.begin() + 1 );
}

BOOST_AUTO_TEST_CASE( const_set_finds_elements_in_insertion_order )
{
    const tools::Set< int > set = boost::assign::list_of( 1 )( 3 )( 2 );
    BOOST_REQUIRE_EQUAL( 3u, set.size() );
    BOOST_CHECK( set.find( 42 ) == set.end() );
    BOOST_CHECK( set.find( 1 ) == set.begin() );
    BOOST_CHECK( set.find( 2 ) == set.begin() + 2 );
    BOOST_CHECK( set.find( 3 ) == set.begin() + 1 );
}

BOOST_AUTO_TEST_CASE( set_finds_const_elements_in_insertion_order )
{
    tools::Set< const int > set = boost::assign::list_of( 1 )( 3 )( 2 );
    BOOST_REQUIRE_EQUAL( 3u, set.size() );
    BOOST_CHECK( set.find( 42 ) == set.end() );
    BOOST_CHECK( set.find( 1 ) == set.begin() );
    BOOST_CHECK( set.find( 2 ) == set.begin() + 2 );
    BOOST_CHECK( set.find( 3 ) == set.begin() + 1 );
}

BOOST_AUTO_TEST_CASE( set_supports_swap )
{
    tools::Set< int > set = boost::assign::list_of( 1 );
    BOOST_CHECK_EQUAL( 1u, set.size() );
    tools::Set< int >().swap( set );
    BOOST_CHECK_EQUAL( 0u, set.size() );
}

BOOST_AUTO_TEST_CASE( set_is_serializable )
{
    std::stringstream s;
    {
        tools::Set< int > set;
        set.insert( 1 );
        boost::archive::text_oarchive a( s );
        a & set;
    }
    {
        tools::Set< int > set;
        boost::archive::text_iarchive a( s );
        a & set;
        BOOST_REQUIRE_EQUAL( 1u, set.size() );
        BOOST_CHECK_EQUAL( 1, *set.begin() );
    }
}
