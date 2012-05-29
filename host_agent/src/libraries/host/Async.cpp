// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Async.h"

using namespace host;

// -----------------------------------------------------------------------------
// Name: Async::Async
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
Async::Async( Pool_ABC& pool )
    : pool_( pool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Async::~Async
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
Async::~Async()
{
    Join();
}

namespace
{
template< typename T, typename U >
void Register( T& access, U& li, const Pool_ABC::Future& data )
{
    boost::lock_guard< T > lock( access );
    li.erase( std::remove_if( li.begin(), li.end(), boost::bind( &Pool_ABC::Future::is_ready, _1 ) ), li.end() );
    li.push_back( data );
}
}

// -----------------------------------------------------------------------------
// Name: Async::Post
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
void Async::Post( const Pool_ABC::Task& task )
{
    Register( access_, futures_, pool_.Post( task ) );
}

// -----------------------------------------------------------------------------
// Name: Async::Go
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
void Async::Go( const Pool_ABC::Task& task )
{
    Register( access_, futures_, pool_.Go( task ) );
}

// -----------------------------------------------------------------------------
// Name: Async::Join
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
void Async::Join()
{
    boost::wait_for_all( futures_.begin(), futures_.end() );
    futures_.clear();
}

// -----------------------------------------------------------------------------
// Name: Async::GetPool
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
Pool_ABC& Async::GetPool() const
{
    return pool_;
}