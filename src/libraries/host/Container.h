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

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/unordered_map.hpp>

namespace host
{
typedef boost::uuids::uuid Uuid;

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
    typedef Container< T > T_Container;
    typedef boost::shared_ptr< T_Object > T_ObjectPtr;
    typedef boost::unordered_map< Uuid, T_ObjectPtr > T_Objects;
    typedef std::function< bool( const T_Object& ) > T_Predicate;

public:
    Container()
    {
        // NOTHING
    }

    virtual ~Container()
    {
        // NOTHING
    }

    template< typename U >
    void Foreach( const U& operand, const T_Predicate& predicate = T_Predicate() ) const
    {
        T_Objects copy;
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        copy = objects_;
        lock.unlock();
        BOOST_FOREACH( const typename T_Objects::value_type& value, copy )
            if( !predicate || predicate( *value.second ) )
                operand( value.second );
    }

    template< typename U >
    static void Adapt( const U& operand, T_ObjectPtr ptr )
    {
        operand( *ptr );
    }

    template< typename U >
    void ForeachRef( const U& operand, const T_Predicate& predicate = T_Predicate() ) const
    {
        Foreach( boost::bind( &T_Container::Adapt< U >, boost::cref( operand ), _1 ), predicate );
    }

    template< typename U >
    static void PushBack( U& list, T_ObjectPtr value )
    {
        list.push_back( value );
    }

    static bool LimitedPredicate( const T_Predicate& predicate, int& offset, int& limit, const T_Object& obj )
    {
        if( predicate && !predicate( obj ) )
            return false;
        return --offset < 0 && limit-- > 0;
    }

    template< typename U >
    std::vector< boost::shared_ptr< U > > List( int offset, int limit, const T_Predicate& predicate = T_Predicate() ) const
    {
        typedef typename std::vector< boost::shared_ptr< U > > List;
        List list;
        Foreach( boost::bind( &T_Container::PushBack< List >, boost::ref( list ), _1 ),
                 boost::bind( &T_Container::LimitedPredicate, predicate, boost::ref( offset ), boost::ref( limit ), _1 ) );
        return list;
    }

    size_t Count() const
    {
        boost::shared_lock< boost::shared_mutex > lock( access_ );
        return objects_.size();
    }

    static void CountOperand( size_t& sum, T_ObjectPtr ptr )
    {
        ++sum;
    }

    size_t Count( const T_Predicate& predicate ) const
    {
        size_t reply = 0;
        Foreach( boost::bind( &T_Container::CountOperand, boost::ref( reply ), _1 ), predicate );
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
        const typename T_Objects::value_type next = std::make_pair( ptr->GetId(), ptr );
        boost::lock_guard< boost::shared_mutex > lock( access_ );
        return objects_.insert( next ).second;
    }

    bool AttachUnless( T_ObjectPtr ptr, const T_Predicate& predicate )
    {
        boost::upgrade_lock< boost::shared_mutex > lock( access_ );
        BOOST_FOREACH( const typename T_Objects::value_type& value, objects_ )
            if( predicate( *value.second ) )
                return false;
        const typename T_Objects::value_type next = std::make_pair( ptr->GetId(), ptr );
        boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
        return objects_.insert( next ).second;
    }

    T_ObjectPtr Detach( const Uuid& id )
    {
        return DetachUnless( id, T_Predicate() );
    }

    T_ObjectPtr DetachUnless( const Uuid& id, const T_Predicate& predicate )
    {
        boost::upgrade_lock< boost::shared_mutex > lock( access_ );
        typename T_Objects::iterator it = objects_.find( id );
        if( it == objects_.end() )
            return T_ObjectPtr();
        if( predicate && predicate( *it->second ) )
            return T_ObjectPtr();
        T_ObjectPtr ptr = it->second;
        boost::upgrade_to_unique_lock< boost::shared_mutex > write( lock );
        objects_.erase( it );
        return ptr;
    }

private:
    mutable boost::shared_mutex access_;
    T_Objects objects_;
};
}

#endif //CONTAINER_H
