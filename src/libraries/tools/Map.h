// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_MAP_H
#define TOOLS_MAP_H

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

namespace tools
{
// =============================================================================
/** @class  Map
    @brief  A container with the properties of a map but sequenced as a vector

    This ensures iteration in the order of insertion.

    One notable application would be to use a pointer as a key without worrying
    about undeterministic iteration order.
*/
// Created: MCO 2013-02-08
// =============================================================================
template< typename K, typename T >
class Map
{
public:
    typedef K key_type;
    typedef T mapped_type;

    struct value_type
    {
        typedef key_type first_type;
        typedef mapped_type second_type;

        explicit value_type( first_type f )
            : first( f )
        {}
        value_type( std::pair< first_type, second_type > p )
            : first( p.first )
            , second( p.second )
        {}
        operator std::pair< first_type, second_type >() const
        {
            return std::make_pair( first, second );
        }
        first_type first;
        mutable second_type second;
    };

private:
    typedef boost::multi_index::multi_index_container<
        value_type,
        boost::multi_index::indexed_by<
            boost::multi_index::random_access<>,
            boost::multi_index::ordered_unique<
                boost::multi_index::member<
                    value_type, key_type, &value_type::first >
            >
        >
    > Container;

public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    mapped_type& operator[]( key_type k )
    {
        return v_.get< 1 >().insert( value_type( k ) ).first->second;
    }
    std::size_t erase( const key_type& k )
    {
        return v_.get< 1 >().erase( k );
    }
    iterator erase( iterator it )
    {
        return v_.erase( it );
    }
    iterator find( key_type k )
    {
        return v_.project< 0 >( v_.get< 1 >().find( k ) );
    }
    const_iterator find( key_type k ) const
    {
        return v_.get< 1 >().find( k );
    }
    std::pair< iterator, bool > insert( value_type v )
    {
        auto result = v_.get< 1 >().insert( v );
        return std::make_pair( v_.project< 0 >( result.first ), result.second );
    }

    iterator begin()
    {
        return v_.begin();
    }
    const_iterator begin() const
    {
        return v_.begin();
    }
    iterator end()
    {
        return v_.end();
    }
    const_iterator end() const
    {
        return v_.end();
    }
    bool empty() const
    {
        return v_.empty();
    }
    std::size_t size() const
    {
        return v_.size();
    }
    void clear()
    {
        v_.clear();
    }

private:
    Container v_;
};

}

#endif // TOOLS_MAP_H
