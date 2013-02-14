// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_SET_H
#define TOOLS_SET_H

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace tools
{
// =============================================================================
/** @class  Set
    @brief  A container with the properties of a set but sequenced as a vector

    This ensures iteration in the order of insertion.

    One notable application would be to use a pointer as a key without worrying
    about undeterministic iteration order.
*/
// Created: MCO 2013-02-08
// =============================================================================
template< typename T >
class Set
{
public:
    typedef typename boost::remove_const< T >::type key_type;
    typedef key_type value_type;

private:
    typedef boost::multi_index::multi_index_container<
        value_type,
        boost::multi_index::indexed_by<
            boost::multi_index::random_access<>,
            boost::multi_index::ordered_unique<
                boost::multi_index::identity< value_type >
            >
        >
    > Container;

public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    Set()
    {}
    template< typename I >
    Set( const I& begin, const I& end )
        : v_( begin, end )
    {}

    std::size_t erase( const key_type& k )
    {
        return v_.get< 1 >().erase( k );
    }
    iterator erase( iterator it )
    {
        return v_.erase( it );
    }
    iterator find( const key_type& k )
    {
        return v_.project< 0 >( v_.get< 1 >().find( k ) );
    }
    const_iterator find( const key_type& k ) const
    {
        return v_.project< 0 >( v_.get< 1 >().find( k ) );
    }
    std::pair< iterator, bool > insert( const value_type& v )
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
    void swap( Set& rhs )
    {
        v_.swap( rhs.v_ );
    }

    template< class Archive >
    void serialize( Archive& archive, const unsigned int /*version*/ )
    {
        archive & v_;
    }

private:
    Container v_;
};

}

#endif // TOOLS_SET_H
