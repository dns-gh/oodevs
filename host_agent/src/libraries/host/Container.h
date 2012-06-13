// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4005 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/unordered_map.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

namespace host
{
typedef boost::uuids::uuid Uuid;

template< typename T >
T Clip( T value, T min, T max )
{
    return std::min( std::max( value, min ), max );
}

// =============================================================================
/** @class  Container
    @brief  Container template class
*/
// Created: BAX 2012-04-17
// =============================================================================
template< typename T >
class Container : public boost::noncopyable
{
public:
    typedef T T_Object;
    typedef boost::shared_ptr< T_Object > T_ObjectPtr;
    typedef boost::unordered_map< Uuid, T_ObjectPtr > T_Objects;
    typedef boost::function< bool( const T_Object& ) > T_Predicate;

public:
    Container()
    {
        // NOTHING
    }

    virtual ~Container()
    {
        // NOTHING
    }

    template< typename T >
    void Foreach( const T& operand, const T_Predicate& predicate = T_Predicate(), int offset = 0, int limit = INT_MAX )
    {
        T_Objects copy;
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        copy = objects_;
        lock.unlock();

        offset = Clip< int >( offset, 0, static_cast< int >( copy.size() ) );
        limit  = Clip< int >( limit, 0, static_cast< int >( copy.size() - offset ) );
        BOOST_FOREACH( const T_Objects::value_type& value, copy )
            if( !predicate || predicate( *value.second ) )
                if( --offset < 0 || limit-- > 0 )
                    operand( *value.second );
    }

    template< typename U >
    std::vector< boost::shared_ptr< U > > List( const T_Predicate& predicate, int offset, int limit )
    {
        std::vector< boost::shared_ptr< U > > reply;
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        offset = Clip< int >( offset, 0, static_cast< int >( objects_.size() ) );
        limit  = Clip< int >( limit, 0, static_cast< int >( objects_.size() ) - offset );
        BOOST_FOREACH( const T_Objects::value_type& value, objects_ )
            if( !predicate || predicate( *value.second ) )
                if( --offset < 0 || limit-- > 0 )
                    reply.push_back( value.second );
        return reply;
    }

    size_t Count() const
    {
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        return objects_.size();
    }

    size_t Count( const T_Predicate& predicate )
    {
        size_t reply = 0;
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        BOOST_FOREACH( const T_Objects::value_type& value, objects_ )
            reply += predicate( *value.second );
        return reply;
    }

    bool Has( const Uuid& id ) const
    {
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        return !!objects_.count( id );
    }

    T_ObjectPtr Get( const Uuid& id ) const
    {
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        typename T_Objects::const_iterator it = objects_.find( id );
        return it == objects_.end() ? T_ObjectPtr() : it->second;
    }

    bool Attach( T_ObjectPtr ptr )
    {
        boost::lock_guard< boost::shared_mutex > lock( access_ );
        return objects_.insert( std::make_pair( ptr->GetId(), ptr ) ).second;
    }

    T_ObjectPtr Detach( const Uuid& id )
    {
        boost::lock_guard< boost::shared_mutex > lock( access_ );
        typename T_Objects::const_iterator it = objects_.find( id );
        if( it == objects_.end() )
            return T_ObjectPtr();
        T_ObjectPtr ptr = it->second;
        objects_.erase( it );
        return ptr;
    }

private:
    mutable boost::shared_mutex access_;
    T_Objects objects_;
};
}

#endif CONTAINER_H
