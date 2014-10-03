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

    std::string ToJson  ( const T_Tree& T_Tree, bool isPretty = true );
    T_Tree      FromJson( const std::string& data );
    T_Tree      FromJson( io::Reader_ABC& stream );

    std::string ToXml   ( const T_Tree& tree );
    T_Tree      FromXml ( const std::string& data );

    template< typename T >
    T Get( const T_Tree& tree, const std::string& key, const T& def = T() )
    {
        const auto data = tree.get_optional< T >( key );
        return data == boost::none ? def : *data;
    }

    template< typename T >
    bool TryRead( T& dst, const T_Tree& tree, const std::string& key )
    {
        const auto data = tree.get_optional< T >( key );
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
        auto& sub = dst.put_child( name, T_Tree() );
        for( auto it = list.begin(); it != list.end(); ++it )
            sub.push_back( std::make_pair( std::string(), boost::lexical_cast< std::string >( *it ) ) );
    }

    template< typename T >
    void PutMap( T_Tree& dst, const std::string& name, const T& map )
    {
        auto& sub = dst.put_child( name, T_Tree() );
        for( auto it = map.begin(); it != map.end(); ++it )
            sub.push_back( std::make_pair( boost::lexical_cast< std::string >( it->first ),
                boost::lexical_cast< std::string >( it->second ) ) );
    }

    template< typename T, typename U >
    void Walk( const T_Tree& src, const T& begin, const T& end, const U& operand )
    {
        auto range = src.equal_range( *begin );
        if( std::distance( begin, end ) > 1 )
            for( auto it = range.first; it != range.second; ++it )
                Walk( it->second, begin + 1, end, operand );
        else
            for( auto it = range.first; it != range.second; ++it )
                operand( it->second );
    }
}

#endif // PROPERTY_TREE_H
