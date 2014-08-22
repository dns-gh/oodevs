// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef TIMER_ABC_H__
#define TIMER_ABC_H__

#include <boost/noncopyable.hpp>
#include <functional>

namespace boost
{
    template< typename T > class shared_ptr;
namespace posix_time
{
    class time_duration;
}
}

namespace runtime
{
    typedef std::function< void( void ) > Task;
    struct Pool_ABC;
// =============================================================================
/** @class  Timer_ABC
    @brief  Timer_ABC class definition
*/
// Created: BAX 2012-06-18
// =============================================================================
struct Timer_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructors
    //@{
              Timer_ABC() {}
     virtual ~Timer_ABC() {}
     //@}

     //! @name Methods
    //@{
     virtual void Stop() = 0;
     //@}
};

typedef boost::shared_ptr< Timer_ABC > Timer;

Timer MakeTimer( Pool_ABC& pool, const boost::posix_time::time_duration& timeout, const Task& task );
}

#endif // TIMER_ABC_H__
