// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Scheduler_h_
#define __Scheduler_h_

#include "Scheduler_ABC.h"
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace mission_tester
{
    class Filter_ABC;
    class Schedulable_ABC;
// =============================================================================
/** @class  Scheduler
    @brief  Scheduler
*/
// Created: PHC 2011-03-28
// =============================================================================
class Scheduler : public Scheduler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Scheduler( boost::shared_ptr< Filter_ABC > filter );
    virtual ~Scheduler();
    //@}

    //! @name Operations
    //@{
    virtual void Schedule( boost::shared_ptr< Schedulable_ABC > schedulable );
    virtual void Step( unsigned int delta, Exercise& exercise );
    //@}

private:
    //! @name types
    //@{
    typedef std::vector< boost::shared_ptr< Schedulable_ABC > > T_Schedulables;
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< Filter_ABC > filter_;
    T_Schedulables schedulables_;
    T_Schedulables::iterator next_;
    boost::posix_time::ptime last_;
    //@}
};
}

#endif // __Scheduler_h_
