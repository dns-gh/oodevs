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

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

namespace io
{
    struct Reader_ABC;
}

namespace property_tree
{
    typedef boost::property_tree::ptree T_Tree;

    std::string ToJson( const T_Tree& T_Tree, bool isPretty = false );
    T_Tree FromJson( const std::string& data );
    T_Tree FromJson( io::Reader_ABC& stream );
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
        const bool modified = dst != *data;
        if( modified )
            dst = *data;
        return modified;
    }

    template< typename T >
    void PutList( T_Tree& dst, const std::string& name, const T& list )
    {
        T_Tree& sub = dst.put_child( name, T_Tree() );
        for( typename T::const_iterator it = list.begin(); it != list.end(); ++it )
            sub.push_back( std::make_pair( std::string(), boost::lexical_cast< std::string >( *it ) ) );
    }

    template< typename T, typename U >
    void Walk( const T_Tree& src, const T& begin, const T& end, const U& operand )
    {
        std::pair< Tree::const_assoc_iterator, Tree::const_assoc_iterator > range = src.equal_range( *begin );
        if( std::distance( begin, end ) > 1 )
            for( Tree::const_assoc_iterator it = range.first; it != range.second; ++it )
                Walk( it->second, begin + 1, end, operand );
        else
            for( Tree::const_assoc_iterator it = range.first; it != range.second; ++it )
                operand( it->second );
    }
}

#endif // PROPERTY_TREE_H
