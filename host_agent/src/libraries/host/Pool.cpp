// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Pool.h"

using namespace host;

// -----------------------------------------------------------------------------
// Name: Pool::Pool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::Pool( size_t numThreads )
    : service_( numThreads )
    , work_   ( service_ )
{
    for( size_t i = 0; i < numThreads; ++i )
        threads_.create_thread( boost::bind( &boost::asio::io_service::run, &service_ ) );
}

// -----------------------------------------------------------------------------
// Name: Pool::~Pool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::~Pool()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: Pool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void Pool::Post( const Task& task )
{
    service_.post( task );
}

// -----------------------------------------------------------------------------
// Name: Pool::Stop
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void Pool::Stop()
{
    service_.stop();
    threads_.join_all();
}
