// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WAIT_EVENT_H__
#define WAIT_EVENT_H__

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/condition_variable.hpp>

namespace dispatcher
{
// =============================================================================
/** @class  WaitEvent
    @brief  WaitEvent class definition
*/
// Created: BAX 2012-06-14
// =============================================================================
struct WaitEvent : public boost::noncopyable
{
    WaitEvent() : signaled_( false )
    {
        // NOTHING
    }

    ~WaitEvent()
    {
        // NOTHING
    }

    void Wait()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        condition_.wait( lock, boost::bind( &WaitEvent::IsSignaledUnlocked, this ) );
    }

    void Signal()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        signaled_ = true;
        condition_.notify_all();
    }

    bool IsSignaled() const
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        return signaled_;
    }

private:
    bool IsSignaledUnlocked() const
    {
        return signaled_;
    }

    mutable boost::mutex access_;
    boost::condition_variable condition_;
    bool signaled_;
};
}

#endif // WAIT_EVENT_H__
