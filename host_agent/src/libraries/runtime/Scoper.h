// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
#ifndef SCOPER_H
#define SCOPER_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace runtime
{
struct Scoper : public boost::noncopyable
{
    typedef boost::function< void( void ) > Task;
 
    Scoper( const Task& task )
        : task_( task )
    {
        // NOTHING
    }
 
    ~Scoper()
    {
        task_();
    }

private:
    const Task task_;
};
}

#endif
