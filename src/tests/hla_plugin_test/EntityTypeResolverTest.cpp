// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "rpr/EntityTypeResolver.h"
#include <xeumeuleu/xml.hpp>

using namespace rpr;

namespace rpr
{
    std::ostream& operator<<( std::ostream& os, const EntityType& type )
    {
        return os << type.str();
    }
}

BOOST_AUTO_TEST_CASE( entity_type_operator_lower_than )
{
    BOOST_CHECK_LE( EntityType(), EntityType( "1" ) );
    BOOST_CHECK_LE( EntityType( "1 0" ), EntityType( "1 0 1" ) );
    BOOST_CHECK_LE( EntityType( "0 0 9" ), EntityType( "3 3 3" ) );
    BOOST_CHECK_LE( EntityType( "1 2 3 4 5 6 7" ), EntityType( "9 8 7 6 5 4 3" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_operator_minus )
{
    BOOST_CHECK_EQUAL( EntityType( "0" ), EntityType( "1" ) - EntityType( "1" ) );
    BOOST_CHECK_EQUAL( EntityType( "0 0 1" ), EntityType( "1 0 1" ) - EntityType( "1 0 0" ) );
    BOOST_CHECK_EQUAL( EntityType( "9 9 9 9 9 9 9" ), EntityType() - EntityType( "9 9 9 9 9 9 9" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_finds_exactly_type )
{
    xml::xistringstream xis( "<dis-mapping default-type='0' default-name='default' threshold='0'>"
                             "    <entry name='exact' dis='1'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( EntityType( "1" ), resolver.Find( "exact" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_finds_approxymatly_type )
{
    xml::xistringstream xis( "<dis-mapping default-type='0' default-name='default' threshold='0'>"
                             "    <entry name='exactly' dis='1'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( EntityType( "1" ), resolver.Find( "other" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_returns_default_type_if_nothing_matchs )
{
    xml::xistringstream xis( "<dis-mapping default-type='0' default-name='default' threshold='0'/>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( EntityType( "0" ), resolver.Find( "other" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_resolves_name_from_exact_type )
{
    xml::xistringstream xis( "<dis-mapping default-type='0' default-name='default' threshold='0'>"
                             "    <entry name='exact' dis='1 2 3'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( "exact", resolver.Resolve( EntityType( "1 2 3" ) ) );
    BOOST_CHECK_EQUAL( "default", resolver.Resolve( EntityType( "1 2 4" ) ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_resolves_name_from_best_matching_type )
{
    xml::xistringstream xis( "<dis-mapping default-type='0'  default-name='default' threshold='9 9 9 9 9 9 9'>"
                             "    <entry name='no-match' dis='4 5 6'/>"
                             "    <entry name='other' dis='1 2 3'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( "other", resolver.Resolve( EntityType( "1 2" ) ) );
    BOOST_CHECK_EQUAL( "other", resolver.Resolve( EntityType( "1 2 0 0 0 0" ) ) );
    BOOST_CHECK_EQUAL( "other", resolver.Resolve( EntityType( "1 2 3 4 5" ) ) );
    BOOST_CHECK_EQUAL( "other", resolver.Resolve( EntityType( "1 2 4" ) ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_returns_default_name_if_nothing_matchs_with_realistic_threshold )
{
    xml::xistringstream xis( "<dis-mapping default-type='0' default-name='default' threshold='0 0 9 1 1 9 9'>"
                             "    <entry name='other' dis='1 2 3'/>"
                             "    <entry name='no-match' dis='4 5 6'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( "default", resolver.Resolve( EntityType( "7 8 9" ) ) );
}
