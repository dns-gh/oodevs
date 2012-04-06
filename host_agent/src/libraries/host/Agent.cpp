// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Agent.h"
#include "Session_ABC.h"
#include "SessionFactory_ABC.h"
#include "Node_ABC.h"
#include "NodeFactory_ABC.h"
#include "cpplog/cpplog.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;

namespace
{
#define CALL_MEMBER( obj, ptr ) ( ( obj ).*( ptr ) )

// -----------------------------------------------------------------------------
// Name: Clip
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T >
T Clip( T value, T min, T max )
{
    assert( min <= max );
    return std::min( std::max( value, min ), max );
}

// -----------------------------------------------------------------------------
// Name: ListObjects
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T, typename U >
Reply ListObjects( T& mutex, const U& objects, int offset, int limit )
{
    std::string data;

    boost::shared_lock< T > lock( mutex );
    typename U::const_iterator it = objects.begin();
    offset = Clip< int >( offset, 0, static_cast< int >( objects.size() ) );
    limit  = Clip< int >( limit,  0, static_cast< int >( objects.size() ) - offset );
    std::advance( it , offset );
    for( int idx = 0; idx < limit; ++idx, ++it )
        data += ( idx ? ", " : "" ) + it->second->ToJson();
    lock.unlock();

    return Reply( "[" + data + "]" );
}

// -----------------------------------------------------------------------------
// Name: CountObjects
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T, typename U >
Reply CountObjects( T& mutex, const U& objects )
{
    boost::shared_lock< T > lock( mutex );
    return Reply( ( boost::format( "{ \"count\" : %1% }" ) % objects.size() ).str() );
}

// -----------------------------------------------------------------------------
// Name: GetObject
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T >
Reply GetObject( boost::shared_mutex& mutex, const T& objects, const std::string& name, const boost::uuids::uuid& id )
{
    boost::shared_lock< boost::shared_mutex > lock( mutex );
    typename T::const_iterator it = objects.find( id );
    if( it == objects.end() )
        return Reply( ( boost::format( "unable to find %1% %2%" ) % name % id ).str(), false );
    return Reply( it->second->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: AddObject
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T, typename U >
void AddObject( boost::shared_mutex& mutex, T& objects, U ptr )
{
    boost::lock_guard< boost::shared_mutex > lock( mutex );
    objects.insert( std::make_pair( ptr->GetTag(), ptr ) );
}

// -----------------------------------------------------------------------------
// Name: ExtractObject
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T, typename U >
T ExtractObject( U& mutex, std::map< boost::uuids::uuid, T >& objects, const boost::uuids::uuid& id )
{
    boost::lock_guard< U > lock( mutex );
    typename std::map< boost::uuids::uuid, T >::iterator it = objects.find( id );
    if( it == objects.end() )
        return T();

    T ptr = it->second;
    objects.erase( it );
    return ptr;
}

// -----------------------------------------------------------------------------
// Name: UuidDispatch
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T, typename U >
Reply UuidDispatch( boost::shared_mutex& mutex, T& objects, const boost::uuids::uuid& id, const std::string& name, U member )
{
    boost::shared_lock< boost::shared_mutex > lock( mutex );
    typename T::const_iterator it = objects.find( id );
    if( it == objects.end() )
        return Reply( ( boost::format( "unable to find %1% %2%" ) % name % id ).str(), false );
    CALL_MEMBER( *it->second, member )();
    return Reply( it->second->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: DeleteObject
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T >
Reply DeleteObject( boost::shared_mutex& mutex, std::map< boost::uuids::uuid, T >& objects, const std::string& name, const boost::uuids::uuid& id )
{
    T ptr = ExtractObject( mutex, objects, id );
    if( !ptr )
        return Reply( ( boost::format( "unable to find %1% %2%" ) % name % id ).str(), false );
    ptr->Stop();
    return Reply( ptr->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: HasObject
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
template< typename T, typename U >
bool HasObject( T& mutex, const U& objects, const boost::uuids::uuid id )
{
    boost::shared_lock< T > lock( mutex );
    return objects.count( id ) == 1;
}

// -----------------------------------------------------------------------------
// Name: DummyLock class
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
struct DummyLock
{
     DummyLock() {}
    ~DummyLock() {}
    void lock() {}
    void unlock() {}
    void lock_shared() {}
    void unlock_shared() {}
};

// -----------------------------------------------------------------------------
// Name: GetFiltered
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
template< typename T >
T GetFiltered( boost::shared_mutex& mutex, const T& data, const boost::uuids::uuid& node )
{
    T filtered;
    boost::shared_lock< boost::shared_mutex > lock( mutex );
    BOOST_FOREACH( typename const T::value_type& value, data )
        if( value.second->GetNode() == node )
            filtered.insert( value );
    return filtered;
}

// -----------------------------------------------------------------------------
// Name: IsOrphanedSession
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
template< typename T >
bool IsOrphanedSession( T& mutex, const Agent::T_Nodes& nodes, const Session_ABC& session )
{
    return !HasObject( mutex, nodes, session.GetNode() );
}

// -----------------------------------------------------------------------------
// Name: IsNode
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
bool IsNode( const Session_ABC& session, const boost::uuids::uuid& node )
{
    return session.GetNode() == node;
}

// -----------------------------------------------------------------------------
// Name: Move
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
template< typename T >
void Move( T& dst, T& src, typename T::iterator it )
{
    dst.insert( *it );
    src.erase( it );
}

// -----------------------------------------------------------------------------
// Name: ExtractSessionsIf
// Created: BAX 2012-04-04
// -----------------------------------------------------------------------------
template< typename T, typename U, typename V >
Agent::T_Sessions ExtractSessionsIf( T& mutex, U& sessions, V predicate )
{
    U reply;
    boost::lock_guard< T > lock( mutex );
    typename U::iterator it = sessions.begin();
    while( it != sessions.end() )
        if( predicate( *it->second ) )
            Move( reply, sessions, it++ );
        else
            ++it;
    return reply;
}
}

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( cpplog::BaseLogger& log, const NodeFactory_ABC& nodeFactory, const SessionFactory_ABC& sessionFactory )
    : log_           ( log )
    , nodeFactory_   ( nodeFactory )
    , sessionFactory_( sessionFactory )
    , access_        ( new boost::shared_mutex() )
    , nodes_         ( nodeFactory_.Reload() )
    , sessions_      ( sessionFactory_.Reload() )
{
    DummyLock dummy;
    ExtractSessionsIf( dummy, sessions_, boost::bind( &IsOrphanedSession< DummyLock >, dummy, nodes_, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::~Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::ListNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::ListNodes( int offset, int limit ) const
{
    return ListObjects( *access_, nodes_, offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::CountNodes() const
{
    return CountObjects( *access_, nodes_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::GetNode( const boost::uuids::uuid& id ) const
{
    return GetObject( *access_, nodes_, "node", id );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::CreateNode( const std::string& name )
{
    boost::shared_ptr< Node_ABC > ptr = nodeFactory_.Create( name );
    if( !ptr )
        return Reply( "unable to create new node", false ); // TODO add better error message
    AddObject( *access_, nodes_, ptr );
    ptr->Start();
    return Reply( ptr->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::DeleteNode( const boost::uuids::uuid& id )
{
    boost::shared_ptr< Node_ABC > ptr;
    T_Sessions invalid;
    {
        DummyLock dummy;
        boost::lock_guard< boost::shared_mutex > lock( *access_ );
        ptr = ExtractObject( dummy, nodes_, id );
        if( !ptr )
            return Reply( ( boost::format( "unable to find node %1%" ) % id ).str(), false );
        invalid = ExtractSessionsIf( dummy, sessions_, boost::bind( &IsNode, _1, id ) );
    }
    // destroy objects outside the lock
    invalid.clear();
    ptr->Stop();
    return Reply( ptr->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::StartNode( const boost::uuids::uuid& id ) const
{
    return UuidDispatch( *access_, nodes_, id, "node", &Node_ABC::Start );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Reply Agent::StopNode( const boost::uuids::uuid& id ) const
{
    return UuidDispatch( *access_, nodes_, id, "node", &Node_ABC::Stop );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::ListSessions( const boost::uuids::uuid& node, int offset, int limit ) const
{
    if( node.is_nil() )
        return ListObjects( *access_, sessions_, offset, limit );

    DummyLock dummy;
    return ListObjects( dummy, GetFiltered( *access_, sessions_, node ), offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CountSessions( const boost::uuids::uuid& node ) const
{
    if( node.is_nil() )
        return CountObjects( *access_, sessions_ );
    DummyLock dummy;
    return CountObjects( dummy, GetFiltered( *access_, sessions_, node ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::GetSession( const boost::uuids::uuid& id ) const
{
    return GetObject( *access_, sessions_, "session", id );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CreateSession( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name )
{
    if( !HasObject( *access_, nodes_, node ) )
        return Reply( "invalid node", false );

    boost::shared_ptr< Session_ABC > ptr = sessionFactory_.Create( node, exercise, name );
    if( !ptr )
        return Reply( "unable to create new session", false ); // TODO add better error message
    AddObject( *access_, sessions_, ptr );
    ptr->Start();
    return Reply( ptr->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::DeleteSession( const boost::uuids::uuid& id )
{
    return DeleteObject( *access_, sessions_, "session", id );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Reply Agent::StartSession( const boost::uuids::uuid& id ) const
{
    return UuidDispatch( *access_, sessions_, id, "session", &Session_ABC::Start );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Reply Agent::StopSession( const boost::uuids::uuid& id ) const
{
    return UuidDispatch( *access_, sessions_, id, "session", &Session_ABC::Stop );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
Reply Agent::ListExercises( int offset, int limit ) const
{
    std::vector< std::string > exercises = sessionFactory_.GetExercises();
    if( exercises.empty() )
        return Reply( "[]" );
    offset = Clip< int >( offset, 0, static_cast< int >( exercises.size() ) );
    if( offset > 0 )
        exercises.erase( exercises.begin(), exercises.begin() + offset );
    limit  = Clip< int >( limit,  0, static_cast< int >( exercises.size() ) );
    exercises.resize( limit );
    return Reply( "[\"" + boost::algorithm::join( exercises, "\", \"" ) + "\"]" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
Reply Agent::CountExercises() const
{
    std::vector< std::string > exercises = sessionFactory_.GetExercises();
    return CountObjects( *access_, exercises );
}
