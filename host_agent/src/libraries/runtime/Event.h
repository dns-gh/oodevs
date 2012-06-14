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

    bool IsSignaled() const
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        return signaled_;
    }

    void Wait()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        if( !signaled_ )
            condition_.wait( lock );
    }

    template< typename T >
    bool Wait( const T& timeout )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        return signaled_ ? true : condition_.timed_wait( lock, timeout );
    }

    void Signal()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        signaled_ = true;
        condition_.notify_all();
    }

private:
    mutable boost::mutex access_;
    boost::condition_variable condition_;
    bool signaled_;
};
}

#endif // EVENT_H__
