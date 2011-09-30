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

#include "Model.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace mission_tester
{
    class Schedulable_ABC;
    class Exercise;

// =============================================================================
/** @class  Scheduler_ABC
    @brief  Scheduler definition
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
    virtual void Schedule( boost::shared_ptr< Schedulable_ABC > schedulable ) = 0;
    virtual void Step( Exercise& exercise, Model& model ) = 0;
    //@}
};

}

#endif // __Scheduler_ABC_h_
