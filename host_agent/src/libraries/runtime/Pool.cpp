// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   pragma warning( disable : 4702 )
#endif

#include "Pool.h"

#include <boost/make_shared.hpp>

using namespace runtime;

namespace
{
struct TaskHandler
{
    typedef boost::packaged_task< void > Package;

    template< typename T >
    explicit TaskHandler( const T& task )
        : package_( boost::make_shared< Package >( task ) )
    {
        // NOTHING
    }

    ~TaskHandler()
    {
        // NOTHING
    }

    void operator()() const
    {
        (*package_)();
    }

    boost::shared_ptr< Package > package_;
};
}

// -----------------------------------------------------------------------------
// Name: Pool::Pool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::Pool( size_t numThreads )
    : work_( service_ )
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

namespace
{
void DetachThread( const Pool::Task& task )
{
    boost::thread detached( task );
}
}

// -----------------------------------------------------------------------------
// Name: Pool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::Future Pool::Post( const Task& task )
{
    TaskHandler handler( task );
    service_.post( handler );
    return Future( handler.package_->get_future() );
}

Pool::Future Pool::Go( const Task& task )
{
    TaskHandler handler( task );
    service_.post( boost::bind( &DetachThread, handler ) );
    return Future( handler.package_->get_future() );
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
