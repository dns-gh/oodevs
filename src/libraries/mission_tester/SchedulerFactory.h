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
             SchedulerFactory();
    virtual ~SchedulerFactory();
    //@}

    //! @name Operations
    //@{
    Scheduler_ABC* CreateAgentScheduler();
    //@}

private:
    //! @name types
    //@{
    typedef std::vector< boost::shared_ptr< Scheduler_ABC > > T_Schedulers;
    //@}

    //! @name Member data
    //@{
    T_Schedulers schedulers_;
    //@}
};
}

#endif // __SchedulerFactory_h_
