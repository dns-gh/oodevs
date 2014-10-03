// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef TREE_HELPERS_H
#define TREE_HELPERS_H

#include "host_test_pch.h"
#include <runtime/PropertyTree.h>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

namespace mocks
{
typedef std::vector< std::function< void( const property_tree::T_Tree& src ) > > T_Constraints;

inline void EqualValue( const property_tree::T_Tree& src, const std::string& key, const std::string& value )
{
    const boost::optional< std::string > opt = src.get_optional< std::string >( key );
    BOOST_REQUIRE( opt != boost::none );
    BOOST_CHECK_EQUAL( *opt, value );
}

inline void EqualTree( const property_tree::T_Tree& src, const std::string& key, const std::string& value )
{
    property_tree::T_Tree::const_assoc_iterator it = src.find( key );
    BOOST_REQUIRE( it != src.not_found() );
    const property_tree::T_Tree expected = property_tree::FromJson( value );
    BOOST_CHECK( it->second == expected );
}

template< typename T >
void Check( const T& list, const property_tree::T_Tree& src )
{
    typedef typename T::value_type Value;
    BOOST_FOREACH( const Value& value, list )
        value( src );
}
}

#endif // TREE_HELPERS_H
