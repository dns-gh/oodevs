// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "Agent.h"
#include "Session.h"
#include "Session_ABC.h"
#include "SessionFactory_ABC.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
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
using namespace runtime;

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( const SessionFactory_ABC& sessionFactory )
    : sessionFactory_( sessionFactory )
    , access_        ( new boost::shared_mutex() )
    , sessions_      ( sessionFactory_.Reload() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::~Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: Clip
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    template< typename T >
    T Clip( T value, T min, T max )
    {
        assert( min <= max );
        return std::min( std::max( value, min ), max );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::ListSessions( int offset, int limit ) const
{
    std::string data;

    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    T_Sessions::const_iterator it = sessions_.begin();
    offset = Clip< int >( offset, 0, static_cast< int >( sessions_.size() ) );
    limit  = Clip< int >( limit,  0, static_cast< int >( sessions_.size() ) - offset );
    std::advance( it , offset );
    for( int idx = 0; idx < limit; ++idx, ++it )
        data += ( idx ? ", " : "" ) + it->second->ToJson();
    lock.unlock();

    return Reply( "[" + data + "]" );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CountSessions() const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    return Reply( ( boost::format( "{ \"count\" : %1% }" ) % sessions_.size() ).str() );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::GetSession( const boost::uuids::uuid& id ) const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    T_Sessions::const_iterator it = sessions_.find( id );
    if( it == sessions_.end() )
        return Reply( ( boost::format( "unable to find session %1%" ) % id ).str(), false );
    return Reply( it->second->ToJson() );
}

namespace
{

// -----------------------------------------------------------------------------
// Name: AddSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
template< typename T, typename U >
void AddSession( T& mutex, U& sessions, boost::shared_ptr< Session_ABC > ptr )
{
    boost::lock_guard< T > lock( mutex );
    sessions.insert( std::make_pair( ptr->GetTag(), ptr ) );
}

// -----------------------------------------------------------------------------
// Name: ExtractSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
template< typename T, typename U >
boost::shared_ptr< Session_ABC > ExtractSession( T& mutex, U& sessions, const boost::uuids::uuid& id )
{
    boost::lock_guard< T > lock( mutex );
    typename U::iterator it = sessions.find( id );
    if( it == sessions.end() )
        return boost::shared_ptr< Session_ABC >();

    boost::shared_ptr< Session_ABC > ptr = it->second;
    sessions.erase( it );
    return ptr;
}

#define CALL_MEMBER( obj, ptr ) ( ( obj ).*( ptr ) )

// -----------------------------------------------------------------------------
// Name: UuidDispatch
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
template< typename T, typename U >
Reply UuidDispatch( boost::shared_mutex& mutex, T& data, const boost::uuids::uuid& id, const std::string& name, U member )
{
    boost::shared_lock< boost::shared_mutex > lock( mutex );
    typename T::const_iterator it = data.find( id );
    if( it == data.end() )
        return Reply( ( boost::format( "unable to find %1% %2%" ) % name % id ).str(), false );
    CALL_MEMBER( *it->second, member )();
    return Reply( it->second->ToJson() );
}

}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CreateSession( const std::string& exercise, const std::string& name )
{
    boost::shared_ptr< Session_ABC > ptr = sessionFactory_.Create( exercise, name );
    if( !ptr )
        return Reply( "unable to create new session", false ); // TODO add better error message
    AddSession( *access_, sessions_, ptr );
    ptr->Start();
    return Reply( ptr->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::DeleteSession( const boost::uuids::uuid& id )
{
    boost::shared_ptr< Session_ABC > ptr = ExtractSession( *access_, sessions_, id );
    if( !ptr )
        return Reply( ( boost::format( "unable to find session %1%" ) % id ).str(), false );
    ptr->Stop();
    return Reply( ptr->ToJson() );
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
// Name: Agent::StartSession
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
    return Reply( ( boost::format("{ \"count\" : %1% }") % exercises.size() ).str() );
}
