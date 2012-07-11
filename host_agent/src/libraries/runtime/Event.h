// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef EVENT_H__
#define EVENT_H__

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/condition_variable.hpp>

namespace runtime
{
// =============================================================================
/** @class  Event
    @brief  Event class definition
*/
// Created: BAX 2012-06-14
// =============================================================================
struct Event : public boost::noncopyable
{
    Event() : signaled_( false )
    {
        // NOTHING
    }

    ~Event()
    {
        // NOTHING
    }

    void Wait()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        condition_.wait( lock, boost::bind( &Event::IsSignaled, this ) );
    }

    template< typename T >
    bool Wait( const T& timeout )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        return condition_.timed_wait( lock, timeout, boost::bind( &Event::IsSignaled, this ) );
    }

    void Signal()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        signaled_ = true;
        condition_.notify_all();
    }

private:
    bool IsSignaled() const
    {
        return signaled_;
    }
    mutable boost::mutex access_;
    boost::condition_variable condition_;
    bool signaled_;
};
}

#endif // EVENT_H__
