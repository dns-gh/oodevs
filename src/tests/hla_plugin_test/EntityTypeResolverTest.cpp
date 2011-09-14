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

BOOST_AUTO_TEST_CASE( entity_type_resolver_finds_exactly_type )
{
    xml::xistringstream xis( "<dis-mapping default='0'>"
                             "    <entry name='exact' dis='1'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( EntityType( "1" ), resolver.Find( "exact" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_finds_approxymatly_type )
{
    xml::xistringstream xis( "<dis-mapping default='0'>"
                             "    <entry name='exactly' dis='1'/>"
                             "</dis-mapping>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( EntityType( "1" ), resolver.Find( "other" ) );
}

BOOST_AUTO_TEST_CASE( entity_type_resolver_returns_default_type_if_nothing_matchs )
{
    xml::xistringstream xis( "<dis-mapping default='0'/>" );
    const EntityTypeResolver resolver( xis );
    BOOST_CHECK_EQUAL( EntityType( "0" ), resolver.Find( "other" ) );
}
