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
#include "Session_ABC.h"
#include "Session.h"
#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
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
Agent::Agent( const Runtime_ABC& runtime )
    : runtime_( runtime )
    , access_ ( new boost::shared_mutex() )
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
Reply Agent::GetSession( const boost::uuids::uuid& tag ) const
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    T_Sessions::const_iterator it = sessions_.find( tag );
    if( it == sessions_.end() )
        return Reply( ( boost::format( "unable to find session %1%" ) % tag ).str(), false );
    return Reply( it->second->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Agent::AddSession( boost::shared_ptr< Session_ABC > ptr )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    sessions_.insert( std::make_pair( ptr->GetTag(), ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::CreateSession( int port )
{
    SessionConfig config;
    config.port = port;
    boost::shared_ptr< Session_ABC > ptr = boost::make_shared< Session >( config );
    if( !ptr )
        return Reply( "unable to create new session", false ); // TODO add better error message
    AddSession( ptr );
    ptr->Start();
    return Reply( ptr->ToJson() );
}

// -----------------------------------------------------------------------------
// Name: Agent::ExtractSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
boost::shared_ptr< Session_ABC > Agent::ExtractSession( const boost::uuids::uuid& tag )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    T_Sessions::iterator it = sessions_.find( tag );
    if( it == sessions_.end() )
        return boost::shared_ptr< Session_ABC >();

    boost::shared_ptr< Session_ABC > ptr = it->second;
    sessions_.erase( it );
    return ptr;
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Reply Agent::DeleteSession( const boost::uuids::uuid& tag )
{
    boost::shared_ptr< Session_ABC > ptr = ExtractSession( tag );
    if( !ptr )
        return Reply( ( boost::format( "unable to find session %1%" ) % tag ).str(), false );

    ptr->Stop();
    return Reply( ptr->ToJson() );
}
