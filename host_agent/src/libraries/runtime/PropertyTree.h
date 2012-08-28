// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include <boost/property_tree/ptree.hpp>

namespace property_tree
{
    typedef boost::property_tree::ptree T_Tree;

    std::string ToJson( const T_Tree& T_Tree, bool isPretty = false );
    T_Tree FromJson( const std::string& data );
    T_Tree FromJson( std::istream& stream );
    std::string ToXml( const T_Tree& tree );
    T_Tree FromXml( const std::string& data );

    template< typename T >
    T Get( const T_Tree& tree, const std::string& key, const T& def = T() )
    {
        const boost::optional< T > data = tree.get_optional< T >( key );
        return data == boost::none ? def : *data;
    }

    template< typename T >
    bool TryRead( T& dst, const T_Tree& tree, const std::string& key )
    {
        const boost::optional< T > data = tree.get_optional< T >( key );
        if( data == boost::none )
            return false;
        dst = *data;
        return true;
    }

    template< typename T >
    void PutList( T_Tree& dst, const std::string& name, const T& list )
    {
        T_Tree& sub = dst.put_child( name, T_Tree() );
        for( typename T::const_iterator it = list.begin(); it != list.end(); ++it )
            sub.push_back( std::make_pair( std::string(), boost::lexical_cast< std::string >( *it ) ) );
    }
}

#endif // PROPERTY_TREE_H
