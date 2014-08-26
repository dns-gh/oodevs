// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef SCOPER_H
#define SCOPER_H

#include <boost/noncopyable.hpp>
#include <cassert>
#include <functional>

namespace runtime
{
// =============================================================================
/** @class  Scoper
    @brief  Scoper class definition
*/
// Created: BAX 2012-06-18
// =============================================================================
struct Scoper : public boost::noncopyable
{
    typedef std::function< void( void ) > Task;
    Scoper( const Task& task )
        : task_( task )
    {
        // NOTHING
    }
    Scoper()
    {
        // NOTHING
    }
    Scoper& operator=( const Task& task )
    {
        assert( !task_ );
        task_ = task;
        return *this;
    }
    void Reset()
    {
        task_ = Task();
    }
    ~Scoper()
    {
        if( task_ )
            task_();
    }
private:
    Task task_;
};
}

#endif
