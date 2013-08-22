// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __AutomatDecisions_ABC_h_
#define __AutomatDecisions_ABC_h_

#include "Extension_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Automat_ABC;

// =============================================================================
/** @class  AutomatDecisions_ABC
    @brief  Automat decisions declaration
*/
// Created: JSR 2012-08-31
// =============================================================================
class AutomatDecisions_ABC : public Extension_ABC
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions_ABC() {}
    virtual ~AutomatDecisions_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool CanBeOrdered() const = 0;
    virtual bool IsEmbraye() const = 0;
    virtual void Engage() = 0;
    virtual void Disengage() = 0;
    //@}
};

}

#endif // __AutomatDecisions_ABC_h_
