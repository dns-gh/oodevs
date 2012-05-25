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
}

#endif // TASK_HANDLER_H
