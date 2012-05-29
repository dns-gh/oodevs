// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Pool.h"

#include <boost/make_shared.hpp>

using namespace host;

namespace
{
template< typename T = void >
struct TaskHandler
{
    typedef boost::packaged_task< T > Package;
    typedef boost::shared_future< T > Future;

    template< typename U >
    explicit TaskHandler( const U& task )
        : package_( boost::make_shared< Package >( task ) )
        , future_ ( package_->get_future() )
    {
    }

    ~TaskHandler()
    {
        // NOTHING
    }

    void operator()() const
    {
        (*package_)();
    }

    Future GetFuture() const
    {
        return future_;
    }

    template< typename U >
    static Future Go( const U& task )
    {
        TaskHandler< T > handler( task );
        boost::thread detached( handler );
        return handler.future_;
    }

private:
    boost::shared_ptr< Package > package_;
    Future future_;
};
}

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

namespace
{
template< typename T >
typename Pool::Future Post( boost::asio::io_service& service, T task )
{
    service.post( task );
    return task.GetFuture();
}
}

// -----------------------------------------------------------------------------
// Name: Pool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::Future Pool::Post( const Task& task )
{
    return ::Post( service_, TaskHandler< void >( task ) );
}

Pool::Future Pool::Go( const Task& task ) const
{
    return TaskHandler< void >::Go( task );
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