// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef TASK_HANDLER_H
#define TASK_HANDLER_H

#include <boost/thread/future.hpp>

namespace host
{
template< typename T >
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

template< typename T = void >
struct Async
{
    Async()
    {
        // NOTHING
    }

    ~Async()
    {
        Join();
    }

    template< typename U >
    void Go( const U& task )
    {
        futures_.push_back( TaskHandler< T >::Go< U >( task ) );
    }

    void Join()
    {
        boost::wait_for_all( futures_.begin(), futures_.end() );
    }

private:
    std::vector< typename TaskHandler< T >::Future > futures_;
};
}

#endif // TASK_HANDLER_H
