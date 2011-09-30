// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Schedulable_ABC_h_
#define __Schedulable_ABC_h_

#include "Model.h"
#include <boost/noncopyable.hpp>

namespace mission_tester
{
    class Exercise;
    class Filter_ABC;

// =============================================================================
/** @class  Schedulable_ABC
    @brief  Schedulable_ABC
*/
// Created: PHC 2011-03-30
// =============================================================================
class Schedulable_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Schedulable_ABC() {}
    virtual ~Schedulable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Matches( const Filter_ABC& filter ) const = 0;
    virtual bool Start( Exercise& exercise, bool withFragOrders ) = 0;
    //virtual bool Recomplete( Exercise& exercise ) = 0;
    //@}
};

}

#endif // __Schedulable_ABC_h_
