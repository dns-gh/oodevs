// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Scheduler_ABC_h_
#define __Scheduler_ABC_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{
    class Schedulable_ABC;
// =============================================================================
/** @class  Scheduler_ABC
    @brief  Scheduler_ABC
*/
// Created: PHC 2011-03-28
// =============================================================================
class Scheduler_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Scheduler_ABC() {}
    virtual ~Scheduler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Schedule( Schedulable_ABC& schedulable ) = 0;
    virtual void Step( unsigned int delta ) = 0;
    //@}
};
}

#endif // __Scheduler_ABC_h_
