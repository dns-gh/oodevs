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
#include <boost/shared_ptr.hpp>

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
    explicit Scheduler( const Filter_ABC& filter );
    virtual ~Scheduler();
    //@}

    //! @name Operations
    //@{
    virtual void Schedule( Schedulable_ABC& schedulable );
    virtual void Step( unsigned int delta );
    //@}

private:
    //! @name Member data
    //@{
    const Filter_ABC& filter_;
    std::vector< Schedulable_ABC* > shedulables_;
    //@}
};
}

#endif // __Scheduler_h_
