// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Agent.h"
#include <runtime/Process_ABC.h>
#include <runtime/Runtime_ABC.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

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
using namespace process;

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( const Runtime_ABC& runtime )
    : runtime_( runtime )
    , access_ ( new boost::shared_mutex() )
{
    BOOST_FOREACH( Runtime_ABC::T_Processes::value_type ptr, runtime_.GetProcesses() )
        processes_.insert( std::make_pair( ptr->GetPid(), ptr ) );
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
    // Name: ToJson
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    std::string ToJson( const Process_ABC& process )
    {
        return (boost::format(
            "{ \"pid\" : %1%, \"name\" : \"%2%\" }" )
        % process.GetPid() % process.GetName() ).str();
    }

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
// Name: Agent::List
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Reply Agent::List( int offset, int limit ) const
{
    std::string data;

    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    T_Processes::const_iterator it = processes_.begin();
    offset = Clip< int >( offset, 0, static_cast< int >( processes_.size() ) );
    limit  = Clip< int >( limit, 0, static_cast< int >( processes_.size() ) - offset );
    std::advance( it, offset );
    for( int idx = 0; idx < limit; ++idx, ++it )
        data += ( idx ? ", " : "" ) + ToJson( *it->second );
    lock.unlock();

    return Reply( "[" + data + "]" );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddProcess
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
void Agent::AddProcess( boost::shared_ptr< Process_ABC > ptr )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    processes_.insert( std::make_pair( ptr->GetPid(), ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::Start
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Reply Agent::Start( const std::string& app, const std::vector< std::string >& args, const std::string& run )
{
    boost::shared_ptr< Process_ABC > ptr = runtime_.Start( app, args, run );
    if( !ptr )
        return Reply( "Unable to create process", false ); // TODO Add LastError()

    AddProcess( ptr );
    return Reply( ToJson( *ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::Extract
// Created: BAX 2012-03-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Process_ABC > Agent::Extract( int pid )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    T_Processes::iterator it = processes_.find( pid );
    if( it == processes_.end() )
        return boost::shared_ptr< Process_ABC >();

    boost::shared_ptr< Process_ABC > reply = it->second;
    processes_.erase( it );
    return reply;
}

// -----------------------------------------------------------------------------
// Name: Agent::Stop
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Reply Agent::Stop( int pid )
{
    boost::shared_ptr< Process_ABC > ptr = Extract( pid );
    if( !ptr )
        return Reply( ( boost::format( "Unable to find process %1%" ) % pid ).str(), false );

    bool done = ptr->Kill( 3 * 1000 );
    if( !done )
        return Reply( ( boost::format( "Unable to kill process %1%" ) % pid ).str(), false ); // TODO Add LastError()

    return Reply( ToJson( *ptr ) );
}
