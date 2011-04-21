// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SchedulerFactory_h_
#define __SchedulerFactory_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace mission_tester
{
    class FilterFactory_ABC;
    class Scheduler_ABC;
// =============================================================================
/** @class  SchedulerFactory
    @brief  SchedulerFactory
*/
// Created: PHC 2011-04-04
// =============================================================================
class SchedulerFactory : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SchedulerFactory( unsigned int delta );
    virtual ~SchedulerFactory();
    //@}

    //! @name Operations
    //@{
    std::auto_ptr< Scheduler_ABC > CreateAgentScheduler();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FilterFactory_ABC > filterFactory_;
    unsigned int                       delta_;
    //@}
};
}

#endif // __SchedulerFactory_h_
