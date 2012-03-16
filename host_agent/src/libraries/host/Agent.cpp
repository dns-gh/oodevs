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
#include <boost/format.hpp>

using namespace host;
using namespace runtime;

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( const Runtime_ABC& runtime )
    : runtime_( runtime )
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
    Runtime_ABC::T_Processes processes = runtime_.GetProcesses();
    offset = Clip< int >( offset, 0, static_cast< int >( processes.size() ) );
    limit  = Clip< int >( limit, 0, static_cast< int >( processes.size() ) - offset );
    Runtime_ABC::T_Processes::const_iterator it = processes.begin();
    std::advance( it , offset );
    std::string data;
    for( int i = 0; i < limit; ++i, ++it )
        data += ( i ? ", " : "" ) + ToJson( **it );
    return Reply( "[" + data + "]" );
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
    return Reply( ToJson( *ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::Stop
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Reply Agent::Stop( int pid )
{
    boost::shared_ptr< Process_ABC > ptr = runtime_.GetProcess( pid );
    if( !ptr )
        return Reply( ( boost::format( "Unable to find process %1%" ) % pid ).str(), false );

    bool done = ptr->Kill( 3 * 1000 );
    if( !done )
        return Reply( ( boost::format( "Unable to kill process %1%" ) % pid ).str(), false ); // TODO Add LastError()

    return Reply( ToJson( *ptr ) );
}
